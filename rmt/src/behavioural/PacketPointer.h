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
 * @file PacketPointer.h
 * TrType Wrapper around pointer to packet
 *
 * Created on: April 12th, 2016
 * Author: Eric Tremblay
 */

#ifndef BEHAVIOURAL_PACKETPOINTER_H_
#define BEHAVIOURAL_PACKETPOINTER_H_

#include <string>
#include "pfpsim/core/TrType.h"

class PacketHeaderVector;

typedef std::size_t TLMAddress;

/**
 * TrType object which wraps around a pointer to a PacketHeaderVector
 * and the address of where the packet is stored in the common data buffer
 */
class PacketPointer : public pfp::core::TrType {
 public:
  PacketPointer() {}
  PacketPointer(int id, TLMAddress addr, std::size_t size)
        : pfp::core::TrType(id), addr(addr), data_size(size) {}

  TLMAddress getPacketAddress() const {return addr;}
  void setPacketAddress(TLMAddress address) {addr = address;}

  virtual std::string data_type() const {return "PacketPointer";}

  std::size_t size() const {return data_size;}

  std::shared_ptr<PacketHeaderVector> phv = nullptr;

 private:
  TLMAddress addr;
  std::size_t data_size;
};

#endif  // BEHAVIOURAL_PACKETPOINTER_H_
