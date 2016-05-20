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

#include "IngressMultiplexer.h"
#include <string>

IngressMultiplexer::IngressMultiplexer(sc_module_name nm,
      pfp::core::PFPObject* parent,
      std::string configfile):IngressMultiplexerSIM(nm, parent, configfile) {
  /*sc_spawn threads*/
  ThreadHandles.push_back(sc_spawn(sc_bind(
        &IngressMultiplexer::IngressMultiplexerThread, this, 0)));
}

void IngressMultiplexer::init() {
    init_SIM(); /* Calls the init of sub PE's and CE's */
}

void IngressMultiplexer::IngressMultiplexer_PortServiceThread() {
}

void IngressMultiplexer::IngressMultiplexerThread(std::size_t thread_id) {
  while (1) {
    if (!packet_in->nb_can_get() && !from_agent->nb_can_get()) {
      wait(packet_in->ok_to_get() | from_agent->ok_to_get());
    } else {
      std::shared_ptr<pfp::core::TrType> received;
      if (from_agent->nb_can_get()) {
        received = from_agent->get();
        if (received->data_type() == "AcceptPackets") {
          npulog(normal, std::cout << "Received begin accepting packets"
                << std::endl;)
          npulog(profile, std::cout << "Received begin accepting packets"
                << std::endl;)
          enable = true;
        } else {
          output->put(received);
        }
      } else if (packet_in->nb_can_get()) {
        if (enable) {
          received = packet_in->get();
          output->put(received);
        } else {
          wait(packet_in->ok_to_get() | from_agent->ok_to_get());
        }
      } else {
        SC_REPORT_ERROR("Ingress Multiplexer", "Impossible Condition!");
      }
    }
  }
}
