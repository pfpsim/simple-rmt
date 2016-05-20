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

#ifndef BEHAVIOURAL_MEMORYMANAGERS_H_
#define BEHAVIOURAL_MEMORYMANAGERS_H_

#include "systemc.h"
#include "tlm.h"
using tlm::tlm_tag;

typedef std::size_t TLMAddress;

class MemoryManagerS : public sc_interface {
 public:
  /* User Logic - Virtual Functions for interface go here */
  virtual TLMAddress write(std::size_t id, char* data, std::size_t
        data_size) = 0;
  virtual void write(TLMAddress addr, std::size_t id, char* data,
        std::size_t data_size) = 0;
  virtual void read(TLMAddress addr, std::size_t& id_out, char*& data_out,
        std::size_t& data_size_out) = 0;
  virtual void free(TLMAddress addr) = 0;
};

#endif  // BEHAVIOURAL_MEMORYMANAGERS_H_
