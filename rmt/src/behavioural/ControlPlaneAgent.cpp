/*
 * simple-rmt: Example RMT simulation model using the PFPSim Framework
 *
 * Copyright (C) 2016 Concordia Univ., Montreal
 *     Samar Abdi
 *     Umair Aftab
 *     Gordon Bailey
 *     Faras Dewal
 *     Shafigh Parsazad
 *     Eric Tremblay
 *
 * Copyright (C) 2016 Ericsson
 *     Bochra Boughzala
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#include "ControlPlaneAgent.h"
#include <vector>
#include <string>
#include "RMTMessages.h"

ControlPlaneAgent::ControlPlaneAgent(sc_module_name nm,
                                     pfp::core::PFPObject* parent,
                                     std::string configfile)
: ControlPlaneAgentSIM(nm, parent, configfile) {
  /*sc_spawn threads*/
  ThreadHandles.push_back(sc_spawn(sc_bind(
        &ControlPlaneAgent::command_processing_thread, this)));
  ThreadHandles.push_back(sc_spawn(sc_bind(
        &ControlPlaneAgent::ControlPlaneAgent_PortServiceThread, this)));
}

void ControlPlaneAgent::init() {
  init_SIM(); /* Calls the init of sub PE's and CE's */
  npulog(profile, cout << "Initializing P4" << endl;)

  // TODO(gordon) - figure out how to get a unique name in case of
  // multiple rmt instances in one simulation
  P4::get("rmt")->init_objects(SPARG("p4"));

  npulog(profile, cout << "P4 init done!" << endl;)
}

void ControlPlaneAgent::ControlPlaneAgent_PortServiceThread() {
  while (1) {
    if (!from_egress->nb_can_get()) {
      wait(from_egress->ok_to_get());
    } else {
      auto received = from_egress->get();
      if (std::dynamic_pointer_cast<RMTMessage>(received)) {
        reply_queue.push(std::dynamic_pointer_cast<RMTMessage>(received));
        reply_received.notify();
      }
    }
  }
}

std::shared_ptr<pfp::cp::CommandResult>
ControlPlaneAgent::send_command(const std::shared_ptr<pfp::cp::Command> & cmd) {
  npulog(profile, cout << "send command start" << endl;)
  current_command = cmd;

  npulog(profile, cout << "notifying new_command" << endl;)
  new_command.notify();

  npulog(profile, cout << "waiting command_processed" << endl;)
  wait(command_processed);

  return current_result;
}

void ControlPlaneAgent::command_processing_thread() {
  while (true) {
    npulog(profile, cout << "waiting new_command" << endl;)
    wait(new_command);

    npulog(profile, cout << "accepting command" << endl;)
    current_result = accept_command(current_command);

    npulog(profile, cout << "notifying command_processed" << endl;)
    command_processed.notify();
  }
}

/* anonymous */namespace {

std::string to_string(const pfp::cp::Bytes & b) {
  return std::string((const char*)b.data(), b.size());
}

bm::MatchKeyParam to_p4_key(const pfp::cp::MatchKey * k) {
  switch (k->get_type()) {
    case pfp::cp::MatchKey::Type::EXACT:
      return bm::MatchKeyParam(
          bm::MatchKeyParam::Type::EXACT,
          to_string(k->get_data()));

    case pfp::cp::MatchKey::Type::LPM:
      return bm::MatchKeyParam(
          bm::MatchKeyParam::Type::LPM,
          to_string(k->get_data()),
          k->get_prefix_len());

    case pfp::cp::MatchKey::Type::TERNARY:
      return bm::MatchKeyParam(
          bm::MatchKeyParam::Type::TERNARY,
          to_string(k->get_data()),
          to_string(k->get_mask()));

    default:
      std::cerr << "Unknown MatchKey::Type " << k->get_type() << std::endl;
      assert(false);
  }
}

}  // end anonymous namespace

std::shared_ptr<pfp::cp::CommandResult>
ControlPlaneAgent::process(pfp::cp::InsertCommand * cmd) {
  cout << "Insert Command at ControlPlaneAgent" << endl;

  // Convert all of the match keys
  std::vector<bm::MatchKeyParam> keys;
  for (const auto & key : cmd->get_keys()) {
    keys.push_back(to_p4_key(key.get()));
  }

  // Convert all of the action parameters
  bm::ActionData action_data;
  for (const pfp::cp::Bytes & b : cmd->get_action().get_params()) {
    action_data.push_back_action_data(bm::Data((const char *)b.data(),
          b.size()));
  }

  std::shared_ptr<AddTableEntry> msg = std::make_shared<AddTableEntry>();

  msg->table_name = cmd->get_table_name();
  msg->match_key = keys;
  msg->action_name = cmd->get_action().get_name();
  msg->action_data = action_data;

  npulog(profile, std::cout
        << "Sending AddTableEntry message to ControlPlaneAgent. "
        << msg->table_name << " --> " << msg->match_key_str << std::endl;)
  npulog(normal, std::cout
        << "Sending AddTableEntry message to ControlPlaneAgent. "
        << msg->table_name << " --> " << msg->match_key_str << std::endl;)
  to_ingress->put(msg);

  wait(reply_received);

  auto reply = reply_queue.pop();

  if (std::dynamic_pointer_cast<TableEntryAdded>(reply)) {
    auto msg = std::dynamic_pointer_cast<TableEntryAdded>(reply);
    return cmd->success_result(msg->handle);
  } else {
    return cmd->failure_result();
  }
}

std::shared_ptr<pfp::cp::CommandResult>
ControlPlaneAgent::process(pfp::cp::ModifyCommand *cmd) {
  /*
  ModifyTableEntry *msg = new ModifyTableEntry();

  msg->table_name = modifyFunctionPointer.table_name;
  msg->action_name = modifyFunctionPointer.action_name;
  msg->action_data = modifyFunctionPointer.action_data;
  msg->handle = modifyFunctionPointer.handle;
  msg->action_data_str = modifyFunctionPointer.action_data_str;

  npulog(profile, std::cout << "Sending ModifyTableEntry message to ControlPlaneAgent. "
        << msg->table_name << " with handle " << msg->handle << std::endl;)
  npulog(normal, std::cout << "Sending ModifyTableEntry message to ControlPlaneAgent. "
        << msg->table_name << " with handle " << msg->handle << std::endl;)
  agent.to_ingress->put(msg);
   */
  cout << "Modify Command at ControlPlaneAgent" << endl;
  return cmd->success_result();
}
std::shared_ptr<pfp::cp::CommandResult>
ControlPlaneAgent::process(pfp::cp::DeleteCommand *cmd) {
  /*
  DeleteTableEntry *msg = new DeleteTableEntry();

  msg->table_name = deleteFunctionPointer.table_name;
  msg->handle = deleteFunctionPointer.handle;

  npulog(profile, std::cout << "Sending DeleteTableEntry message to ControlPlaneAgent. "
        << msg->table_name << " with handle " << msg->handle << std::endl;)
  npulog(normal, std::cout << "Sending DeleteTableEntry message to ControlPlaneAgent. "
        << msg->table_name << " with handle " << msg->handle << std::endl;)
  agent.to_ingress->put(msg);
   */
  cout << "Delete Command at ControlPlaneAgent" << endl;
  return cmd->success_result();
}

std::shared_ptr<pfp::cp::CommandResult>
ControlPlaneAgent::process(pfp::cp::BootCompleteCommand*) {
  std::shared_ptr<AcceptPackets> msg = std::make_shared<AcceptPackets>();
  npulog(profile, std::cout
        << "Sending AcceptPackets message to ControlPlaneAgent."
        << std::endl;)
  npulog(normal, std::cout
        << "Sending AcceptPackets message to ControlPlaneAgent."
        << std::endl;)
  to_ingress->put(msg);
  return nullptr;
}
