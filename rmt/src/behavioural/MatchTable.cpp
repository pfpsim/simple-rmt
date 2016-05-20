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

#include "MatchTable.h"
#include <string>
#include "MatchStage.h"
#include "PacketHeaderVector.h"
#include "RMTMessages.h"

MatchTable::MatchTable(sc_module_name nm, pfp::core::PFPObject* parent,
      std::string configfile):MatchTableSIM(nm, parent, configfile) {
  /*sc_spawn threads*/
  ThreadHandles.push_back(sc_spawn(sc_bind(&MatchTable::MatchTableThread,
        this, 0)));
}

void MatchTable::init() {
    init_SIM(); /* Calls the init of sub PE's and CE's */
}

void MatchTable::MatchTable_PortServiceThread() {
}

void MatchTable::MatchTableThread(std::size_t thread_id) {
  std::string module_stack = parent_->GetParent()->module_name() + "->"
        + parent_->module_name() + "->" + module_name();
  MatchStage* parent = dynamic_cast<MatchStage*>(parent_);
  while (1) {
    if (!table_in->nb_can_get()) {
      wait(table_in->ok_to_get());
    } else {
      // read input
      auto received_pkt = table_in->get();
      if (received_pkt->data_type() == "PacketHeaderVector") {
        std::shared_ptr<PacketHeaderVector> phv =
              std::dynamic_pointer_cast<PacketHeaderVector>(received_pkt);
        npulog(profile, std::cout << module_stack << " received packet "
              << phv->id() << std::endl;)

        // Check if this stage is configured
        if (parent->has_config) {
          MatchStageConfig &config = parent->config;
          std::string stage_name = config.name;
          npulog(debug, cout << module_stack << " has a config. Stage Name: "
                << stage_name << endl;)
          // Check if the phv must be applied to this stage
          if (phv->get_next_table() != ""
                && stage_name == phv->get_next_table()) {
            // applying packet to match stage
            bm::ControlFlowNode* control_flow_node = P4::get("rmt")->
                  get_p4_objects()->get_control_node(stage_name);
            npulog(profile, std::cout << module_stack
                  << " performing lookup on packet " << phv->id() << " ("
                  << stage_name << ")"<< std::endl;)
            npulog(normal, std::cout << module_stack
                  << " performing lookup on packet " << phv->id() << " ("
                  << stage_name << ")"<< std::endl;)
            const bm::ControlFlowNode* next_control_flow_node =
                  (*control_flow_node)(phv->packet().get());
            if (next_control_flow_node != 0) {
              std::string next_stage_name;
              const bm::MatchActionTable* match_table =
                    dynamic_cast<const bm::MatchActionTable*>
                    (next_control_flow_node);
              const bm::Conditional* cond =
                  dynamic_cast<const bm::Conditional*>(next_control_flow_node);
              if (match_table != 0) {
                next_stage_name = match_table->get_name();
              } else if (cond != 0) {
                next_stage_name = cond->get_name();
              }
              npulog(profile, cout << module_stack
                    << " setting next stage for packet " << phv->id() << " to "
                    << next_stage_name << endl;)
              phv->set_next_table(next_stage_name);
            } else {
              npulog(profile, cout << module_stack
                    << " - no next stage for packet " << phv->id() << endl;)
              phv->set_next_table("");
            }
            wait(1, SC_NS);
          } else {
            npulog(profile, cout << module_stack
                  << " is not the next stage for packet " << phv->id()
                  << ". Writing packet to output port." << endl;)
          }
        } else {
          npulog(profile, cout << module_stack
                << " is an empty stage. Writing packet to output port."
                << endl;)
        }
        if (!table_out->nb_can_put()) {
          wait(table_out->ok_to_put());
        }
        // Write packet
        npulog(profile, std::cout << module_stack << " wrote packet "
              << phv->id() << std::endl;)
        table_out->put(received_pkt);
      } else {
        std::shared_ptr<RMTMessage> msg
              = std::dynamic_pointer_cast<RMTMessage>(received_pkt);
        npulog(profile, std::cout << module_stack << " received message "
              << received_pkt->id() << std::endl;)
        if (parent->has_config) {
          MatchStageConfig &config = parent->config;
          std::string stage_name = config.name;
          if (msg->table_name == stage_name) {
            npulog(profile, std::cout << module_stack << " executing message "
                  << received_pkt->id() << " (" << msg->table_name << ")"
                  << std::endl;)
            npulog(normal, std::cout << module_stack << " executing message "
                  << received_pkt->id() << " (" << msg->table_name << ")"
                  << std::endl;)
            msg->execute(*this);
          } else {
            // Message is not for this table
            table_out->put(received_pkt);
          }
        } else {
          npulog(debug, std::cout << module_stack << " is not configured!"
                << std::endl;)
          table_out->put(received_pkt);
        }
      }
    }
  }
}
