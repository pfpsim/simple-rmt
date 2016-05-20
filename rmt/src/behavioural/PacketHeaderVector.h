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

/**
* PacketHeaderVector.h
*
* Created on: March 8, 2016
* Author: Eric Tremblay
*/
/**
 * @file PacketHeaderVector.h
 * Wrapper around P4 packet and phv objects for RMT
 */
#ifndef BEHAVIOURAL_PACKETHEADERVECTOR_H_
#define BEHAVIOURAL_PACKETHEADERVECTOR_H_

#include <string>
#include <vector>
#include "common/P4.h"
#include "pfpsim/core/PacketBase.h"

namespace bm {
class Packet;
}

typedef std::size_t TLMAddress;

class PacketHeaderVector: public pfp::core::PacketBase {
 public:
  PacketHeaderVector(std::size_t id, std::vector<char> raw_header,
      std::string table = "")
      : size(raw_header.size()),
        p4_packet(P4::get("rmt")->new_packet_ptr(0/*ingress_port*/,
          id, raw_header.size(),
          bm::PacketBuffer(
              raw_header.size(),  // capacity (TODO(gordon) be less arbitrary)
              raw_header.data(),
              raw_header.size()))),
          PacketBase(id, "PacketHeaderVector"),
          next_table(table) {}

  virtual ~PacketHeaderVector() = default;

  bm::PHV* phv() {return p4_packet->get_phv();}

  std::shared_ptr<bm::Packet> packet() {return p4_packet;}

  void set_next_table(std::string table) {next_table = table;}
  std::string get_next_table() const {return next_table;}

  void setPayloadAddress(TLMAddress addr) {payload_addr = addr;}
  TLMAddress getPayloadAddress() const {return payload_addr;}

  bool debuggable() const override {return true;}

 private:
  std::size_t size;
  std::shared_ptr<bm::Packet> p4_packet;  // p4 packet object
  std::string next_table;  // next table or conditional to apply to packet
  TLMAddress payload_addr;
};

#endif  // BEHAVIOURAL_PACKETHEADERVECTOR_H_
