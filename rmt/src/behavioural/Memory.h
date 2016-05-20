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

#ifndef BEHAVIOURAL_MEMORY_H_
#define BEHAVIOURAL_MEMORY_H_
#include <map>
#include <utility>
#include <string>
#include "../structural/MemorySIM.h"
#include "MemI.h"

template<typename T>
class Memory:
public MemI<T>,
public MemorySIM {
 public:
  explicit Memory(sc_module_name nm, pfp::core::PFPObject* parent = 0,
        std::string configfile = "");

  /* User Implementation of the Virtual Functions in the Interface.h file */
  virtual void write(TLMAddress addr, T data);
  virtual T read(TLMAddress addr);

 private:
  std::map<TLMAddress, T> memory_map;
  sc_mutex rw_mutex;
  sc_time read_delay;
  sc_time write_delay;
};

/*
	Memory Consturctor
 */
template<typename T>
Memory<T>::Memory(sc_module_name nm, pfp::core::PFPObject* parent,
      std::string configfile):MemorySIM(nm, parent, configfile),
      read_delay(10, SC_NS), write_delay(20, SC_NS) {
}

template<typename T>
void Memory<T>::write(TLMAddress addr, T data) {
  rw_mutex.lock();
  auto obj = memory_map.find(addr);
  if (obj != memory_map.end()) {
    memory_map.erase(obj);
  }
  memory_map.insert(std::pair<TLMAddress, T>(addr, data));
  // wait(write_delay);
  rw_mutex.unlock();
}

template<typename T>
T Memory<T>::read(TLMAddress addr) {
  rw_mutex.lock();
  T data = nullptr;
  auto result = memory_map.find(addr);
  if (result != memory_map.end()) {
    data = result->second;
  }
  // wait(read_delay);
  rw_mutex.unlock();
  return data;
}

#endif  // BEHAVIOURAL_MEMORY_H_
