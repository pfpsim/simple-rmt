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

#include "Deparser.h"
#include <string>
#include "PacketHeaderVector.h"
#include "PacketPointer.h"

Deparser::Deparser(sc_module_name nm, pfp::core::PFPObject* parent,
      std::string configfile):DeparserSIM(nm, parent, configfile) {
  /*sc_spawn threads*/
  ThreadHandles.push_back(sc_spawn(sc_bind(&Deparser::DeparserThread,
        this, 0)));
}

void Deparser::init() {
    init_SIM(); /* Calls the init of sub PE's and CE's */
}

void Deparser::Deparser_PortServiceThread() {
}

void Deparser::DeparserThread(std::size_t thread_id) {
  std::string module_stack = parent_->module_name() + "->" + module_name();
  while (1) {
    if (!deparser_in->nb_can_get()) {
      wait(deparser_in->ok_to_get());
    } else {
      // Read input
      auto received = deparser_in->get();
      if (received->data_type() == "PacketHeaderVector") {
        // Cast to PHV
        auto phv = std::dynamic_pointer_cast<PacketHeaderVector>(received);
        npulog(profile, std::cout << module_stack << " received packet "
              << phv->id() << std::endl;)

        // Get payload size while its available
        const std::size_t payload_size =
              phv->packet().get()->get_payload_size();

        // Perform deparsing
        P4::get("rmt")->get_deparser("deparser")->deparse(phv->packet().get());
        npulog(profile, std::cout << module_stack << " deparsed packet "
              << phv->id() << std::endl;)
        npulog(normal, std::cout << module_stack << " deparsed packet "
              << phv->id() << std::endl;)

        // Get address of payload in common data buffer
        TLMAddress payload_addr = phv->getPayloadAddress();

        // TrType to be sent out
        std::shared_ptr<pfp::core::TrType> out = nullptr;

        if (parent_->module_name() == "ingress_pipeline") {
          // Store the entire packet in common data buffer at
          // the location of the packet's payload
          memory_manager->write(payload_addr, phv->id(),
                phv->packet().get()->data(),
                phv->packet().get()->get_data_size());
          // Create object which holds the location of the packet
          // in the common data buffer. It also holds a pointer to the
          // PacketHeaderVector object which is necessary to maintain
          // the bm::Packet object which seems to have important metadata
          // that cannot be recreated using the packet data alone.
          std::shared_ptr<PacketPointer> pkt_ptr
                = std::make_shared<PacketPointer>(phv->id(), payload_addr,
                    phv->packet().get()->get_data_size());
          pkt_ptr->phv = phv;
          out = pkt_ptr;
        } else if (parent_->module_name() == "egress_pipeline") {
          // called packet because currently, the entire packet is stored,
          // not just the payload
          std::size_t packet_id;
          char* packet = nullptr;
          std::size_t packet_size;
          // Read packet from common data buffer
          memory_manager->read(payload_addr, packet_id, packet, packet_size);
          // Free memory pool block so it can be reused.
          memory_manager->free(payload_addr);

          // This part simply compares the payload of the packet read from
          // the common data buffer and of the packet in the PacketHeaderVector
          // to ensure that it was stored and retreived correctly.
          char* phv_payload = phv->packet().get()->payload();
          char* mem_payload = packet + (packet_size - payload_size);
          if ((memcmp(phv_payload, mem_payload, sizeof(*mem_payload)) == 0)
                && (phv->id() == packet_id)) {
            npulog(normal, std::cout << "packet " << packet_id
                  << "'s payload was retrieved correctly." << std::endl;)
          } else {
            npulog(normal, std::cout << "packet " << packet_id
                  << "'s payload was retrieved incorrectly." << std::endl;)
          }
          out = phv;
        }

        // Wait until the deparser can put
        if (!deparser_out->nb_can_put()) {
          wait(deparser_out->ok_to_put());
        }
        // write packet
        npulog(profile, std::cout << module_stack << " wrote packet "
              << phv->id() << std::endl;)
        deparser_out->put(out);
      } else {
        npulog(profile, std::cout << module_stack << " received message "
              << received->id() <<std::endl;)
        deparser_out->put(received);
      }
    }
  }
}
