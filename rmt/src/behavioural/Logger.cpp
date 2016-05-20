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

#include "Logger.h"
#include <string>
#include <vector>
#include "PacketHeaderVector.h"
#include "common/PcapLogger.h"

Logger::Logger(sc_module_name nm,
      pfp::core::PFPObject* parent,
      std::string configfile):LoggerSIM(nm, parent, configfile) {
  /*sc_spawn threads*/
  ThreadHandles.push_back(sc_spawn(sc_bind(
        &Logger::Logger_PortServiceThread, this)));
}

void Logger::init() {
  init_SIM(); /* Calls the init of sub PE's and CE's */
}

void Logger::Logger_PortServiceThread() {
  std::string outputfile = SPARG("validation-out");
  PcapLogger logger(outputfile);
  while (1) {
    if (!in->nb_can_get()) {
      wait(in->ok_to_get());
    } else {
      auto p = in->get();
      if (std::dynamic_pointer_cast<PacketHeaderVector>(p)) {
        std::shared_ptr<PacketHeaderVector> phv
              = std::dynamic_pointer_cast<PacketHeaderVector>(p);
        npulog(profile, cout << module_name() << " received packet "
              << phv->id() << endl;)
        npulog(normal, cout << module_name() << " received packet "
              << phv->id() << endl;)
        std::vector<uint8_t> data;
        char *packet_data = phv->packet()->data();
        int data_size = phv->packet()->get_data_size();
        for (int i = 0; i < data_size; i++) {
          data.push_back(static_cast<uint8_t>(packet_data[i]));
        }
        logger.logPacket(data);
        // delete phv;
      } else {
        std::cerr << "Logger Packet Dynamic Cast Failed" << endl;
        SC_REPORT_ERROR("PACKET Logger", "Logger Dynamic Cast Failed");
        sc_stop();
      }
    }
  }
}

void Logger::LoggerThread(std::size_t thread_id) {
}
