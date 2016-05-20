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

#include "Parser.h"
#include <vector>
#include <string>
#include <algorithm>
#include "PacketHeaderVector.h"
#include "common/InputStimulus.h"
#include "PacketPointer.h"

Parser::Parser(sc_module_name nm, pfp::core::PFPObject* parent,
      std::string configfile):ParserSIM(nm, parent, configfile) {
  /*sc_spawn threads*/
  ThreadHandles.push_back(sc_spawn(sc_bind(&Parser::ParserThread, this, 0)));
}

void Parser::init() {
  init_SIM(); /* Calls the init of sub PE's and CE's */
}

void Parser::Parser_PortServiceThread() {
}

void Parser::ParserThread(std::size_t thread_id) {
  std::string module_stack = parent_->module_name() + "->" + module_name();
  while (1) {
    if (!parser_in->nb_can_get()) {
      wait(parser_in->ok_to_get());
    } else {
      // Read input
      auto received = parser_in->get();
      std::shared_ptr<PacketHeaderVector> packet = nullptr;
      if (received->data_type() == "InputStimulus") {  // ingress only
        npulog(profile, std::cout << module_stack << " received packet "
              << received->id() << std::endl;)
        // Cast to input stimulus
        auto input_obj = (std::dynamic_pointer_cast<InputStimulus>(received))
              .get()->stimulus_object();
        // Extract packet from input stimulus
        auto received_packet = std::get<0>(input_obj);
        // Split packet header and payload
        auto start_of_header = received_packet.data().begin();
        auto end_of_header = received_packet.data().end();
        // Extract header into new buffer
        std::vector<char> raw_header;
        std::copy(start_of_header, end_of_header,
              std::back_inserter(raw_header));

        // Create packet header object
        packet = std::make_shared<PacketHeaderVector>
                (received_packet.id(), raw_header);
      } else if (received->data_type() == "PacketPointer") {  // egress only
        // Cast to PacketPointer
        auto pkt_ptr = std::dynamic_pointer_cast<PacketPointer>(received);
        // Get address of packet in common data buffer
        TLMAddress packet_addr = pkt_ptr->getPacketAddress();
        std::size_t packet_id;
        char* packet_data = nullptr;
        std::size_t packet_size;
        // Read packet from common data buffer.
        // This is performed for modeling purposes since it will
        // not actually be used. This is because reconstructing the bm::Packet
        // from this packet does not maintain important metadata.
        // Thus, the original bm::Packet that was created originally
        // in ingress parser will be used.
        memory_manager->read(packet_addr, packet_id, packet_data, packet_size);
        // Assign PacketHeaderVector object saved in PacketPointer.
        packet = pkt_ptr->phv;
      } else {
        // The parser has nothing to do with this TrType. Simply send it out.
        npulog(profile, std::cout << module_stack << " forwarding message "
              << received->id() << std::endl;)
        npulog(normal, std::cout << module_stack << " forwarding message "
              << received->id() << std::endl;)
        parser_out->put(received);
        continue;
      }

      // Perform parsing
      int num_states = P4::get("rmt")->get_parser("parser")
            ->parse(packet->packet().get());

      // wait number of states * 1 ns
      wait(num_states, SC_NS);

      npulog(profile, std::cout << module_stack << " parsed packet "
            << packet->id() << " (" << num_states << " states)" << std::endl;)
      npulog(normal, std::cout << module_stack << " parsed packet "
            << packet->id() << " (" << num_states << " states)" << std::endl;)

      if (parent_->module_name() == "ingress_pipeline") {
        // Get payload from bm::Packet
        char* payload = packet->packet().get()->data();
        // Get payload size from bm::Packet
        size_t data_size = packet->packet().get()->get_data_size();
        // Set payload size attribute in bm::Packet (must be done manually)
        packet->packet().get()->set_payload_size(data_size);
        // Save payload of packet in common data buffer
        TLMAddress payload_addr = memory_manager->write(packet->id(),
              payload, data_size);
        // Save the address of the payload in the PacketHeaderVector
        packet->setPayloadAddress(payload_addr);
      } else if (parent_->module_name() == "egress_pipeline") {
        // The payload is already stored in the common data buffer
        // so there is no need to do it again
        // This is kept in case it is need in the future
      }

      // Set first table of packet to pipeline's first table
      npulog(profile, std::cout << module_stack
            << " setting next stage of packet " << packet->id() << " to "
            << first_table << std::endl;)
      packet->set_next_table(first_table);

      // Wait until the parser can put
      if (!parser_out->nb_can_put()) {
        wait(parser_out->ok_to_put());
      }
      // Write packet to next module
      npulog(profile, std::cout << module_stack << " wrote packet "
            << packet->id() << std::endl;)
      parser_out->put(packet);
    }
  }
}

void Parser::set_first_table(std::string table) {
  first_table = table;
}
