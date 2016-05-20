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

#include "Selector.h"
#include <string>
#include <vector>
#include "MatchStage.h"

Selector::Selector(sc_module_name nm, pfp::core::PFPObject* parent,
      std::string configfile):SelectorSIM(nm, parent, configfile) {
  /*sc_spawn threads*/
  ThreadHandles.push_back(sc_spawn(sc_bind(&Selector::SelectorThread,
        this, 0)));
}

void Selector::init() {
    init_SIM(); /* Calls the init of sub PE's and CE's */
}

void Selector::Selector_PortServiceThread() {
}

void Selector::SelectorThread(std::size_t thread_id) {
  MatchStage* parent = dynamic_cast<MatchStage*>(parent_);
  std::string module_stack = parent_->GetParent()->module_name() + "->"
        + parent_->module_name() + "->" + module_name();
  while (1) {
    if (!select_in->nb_can_get()) {
      wait(select_in->ok_to_get());
    } else {
      // Read input
      auto received = select_in->get();
      std::string received_type;
      if (received->data_type() == "PacketHeaderVector") {
        received_type = "packet";
      } else {
        received_type = "message";
      }
      npulog(profile, std::cout << module_stack << " received "
            << received_type << " " << received->id() << std::endl;)
      if (parent->has_config) {
        wait(1, SC_NS);
      }
      if (!select_out->nb_can_put()) {
        wait(select_out->ok_to_put());
      }
      npulog(profile, std::cout << module_stack << " wrote " << received_type
            << " " << received->id() << std::endl;)
      select_out->put(received);
    }
  }
}
