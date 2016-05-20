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

#include "EgressDemultiplexer.h"
#include <string>

EgressDemultiplexer::EgressDemultiplexer(sc_module_name nm,
        pfp::core::PFPObject* parent, std::string configfile)
        :EgressDemultiplexerSIM(nm, parent, configfile) {
  /*sc_spawn threads*/
  ThreadHandles.push_back(sc_spawn(sc_bind(
        &EgressDemultiplexer::EgressDemultiplexerThread, this, 0)));
}

void EgressDemultiplexer::init() {
    init_SIM(); /* Calls the init of sub PE's and CE's */
}

void EgressDemultiplexer::EgressDemultiplexer_PortServiceThread() {
}

void EgressDemultiplexer::EgressDemultiplexerThread(std::size_t thread_id) {
  while (1) {
    if (!input->nb_can_get()) {
      wait(input->ok_to_get());
    } else {
      // read input
      auto received = input->get();
      if (received->data_type() == "PacketHeaderVector") {
        npulog(profile, std::cout << " Forwarding packet " << received->id()
              << " to egress." << std::endl;)
        packet_out->put(received);
      } else {
        npulog(profile, std::cout << " Forwarding message " << received->id()
              << " to agent." << std::endl;)
        to_agent->put(received);
      }
    }
  }
}
