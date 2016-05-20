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

#ifndef BEHAVIOURAL_TCAMMEMORY_H_
#define BEHAVIOURAL_TCAMMEMORY_H_
#include <string>
#include <vector>
#include "../structural/TcamMemorySIM.h"
#include "TcamMemI.h"

const std::string module_name_ = "TcamMemory";

// #define debug_tlm_mem_transaction 1
template<typename T>
class TcamMemory:
public TcamMemI<T>,
public TcamMemorySIM {
 public:
  /* CE Consturctor */
  explicit TcamMemory(sc_module_name nm, pfp::core::PFPObject* parent = 0,
        std::string configfile = "");
  /* User Implementation of the Virtual Functions in the Interface.h */

  /* User Implementation of the Virtual Functions in the MemI.h file */

  virtual void write(TcamActionBase* action, unsigned int addr) {
    if (addr < SIZE) {
      wait(WR_LATENCY);
      // npulog(profile, std::cout << "Writing to MEM @ addr " << std::endl;)
      mem[addr] = action;
    } else {
      SC_REPORT_ERROR("Tcam Memory Write", "Out of address range!");
    }
  }

  virtual TcamActionBase* read(unsigned int addr) {
    if (addr < SIZE) {
      wait(RD_LATENCY);
      return mem[addr];
    } else {
      SC_REPORT_ERROR("Tcam Memory Read", "Out of address range!");
     }
  }

 private:
  std::string modulename;
  TcamActionBase** mem;
  uint64_t SIZE;
  sc_time RD_LATENCY;
  sc_time WR_LATENCY;
};

/*
	TcamMemory Consturctor
 */
// TODO(eric) add config file for memory --> "TSE_Mem_Config.cfg"
template<typename T>
TcamMemory<T>::TcamMemory(sc_module_name nm, pfp::core::PFPObject* parent,
      std::string configfile):TcamMemorySIM(nm, parent, configfile) {
  RD_LATENCY = sc_time(10, SC_NS);
  WR_LATENCY = sc_time(20, SC_NS);
  SIZE = 1024;
  mem = new TcamActionBase*[SIZE];  // allocate the block on host mem in heap
}

#endif  // BEHAVIOURAL_TCAMMEMORY_H_
