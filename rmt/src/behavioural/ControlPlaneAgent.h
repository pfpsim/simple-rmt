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

#ifndef BEHAVIOURAL_CONTROLPLANEAGENT_H_
#define BEHAVIOURAL_CONTROLPLANEAGENT_H_
#include <vector>
#include <string>
#include "structural/ControlPlaneAgentSIM.h"
#include "ControlPlaneAgentS.h"
#include "pfpsim/core/cp/Commands.h"
#include "RMTMessages.h"

class ControlPlaneAgent: public ControlPlaneAgentSIM,
                         public pfp::cp::CommandProcessor {
 public:
  SC_HAS_PROCESS(ControlPlaneAgent);
  /*Constructor*/
  explicit ControlPlaneAgent(sc_module_name nm,
                             pfp::core::PFPObject* parent = 0,
                             std::string configfile = "");
  /*Destructor*/
  virtual ~ControlPlaneAgent() = default;

 public:
  void init();

  std::shared_ptr<pfp::cp::CommandResult>
  send_command(const std::shared_ptr<pfp::cp::Command> & cmd) override;

  std::shared_ptr<pfp::cp::CommandResult> process(
        pfp::cp::InsertCommand*) override;
  std::shared_ptr<pfp::cp::CommandResult> process(
        pfp::cp::ModifyCommand*) override;
  std::shared_ptr<pfp::cp::CommandResult> process(
        pfp::cp::DeleteCommand*) override;
  std::shared_ptr<pfp::cp::CommandResult> process(
        pfp::cp::BootCompleteCommand*) override;

  void command_processing_thread();

 private:
  void ControlPlaneAgent_PortServiceThread();
  void ControlPlaneAgentThread(std::size_t thread_id);
  std::vector<sc_process_handle> ThreadHandles;

  sc_event new_command;
  std::shared_ptr<pfp::cp::Command> current_command;

  sc_event command_processed;
  std::shared_ptr<pfp::cp::CommandResult> current_result;

  MTQueue<std::shared_ptr<RMTMessage>> reply_queue;
  sc_event reply_received;
};
#endif  // BEHAVIOURAL_CONTROLPLANEAGENT_H_
