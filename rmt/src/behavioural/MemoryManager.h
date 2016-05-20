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

#ifndef BEHAVIOURAL_MEMORYMANAGER_H_
#define BEHAVIOURAL_MEMORYMANAGER_H_
#include <string>
#include <vector>
#include "../structural/MemoryManagerSIM.h"

class MemoryPool;
class MemoryValue;

class MemoryManager: public MemoryManagerSIM {
 public:
  SC_HAS_PROCESS(MemoryManager);
  /*Constructor*/
  MemoryManager(sc_module_name nm , int memPortSize ,
        pfp::core::PFPObject* parent = 0,
        std::string configfile = "");
  /*Destructor*/
  virtual ~MemoryManager() = default;

 public:
  void init();
  virtual TLMAddress write(std::size_t id, char* data, std::size_t data_size);
  virtual void write(TLMAddress addr, std::size_t id, char* data,
        std::size_t data_size);
  virtual void read(TLMAddress addr, std::size_t& id_out, char*& data_out,
        std::size_t& data_size_out);
  virtual void free(TLMAddress addr);

 private:
  class WriteArgs {
   public:
    WriteArgs() {}
    WriteArgs(TLMAddress addr, std::size_t id, char* data,
          std::size_t data_size)
          : addr(addr), id(id), data(data), data_size(data_size) {}

    TLMAddress addr;
    std::size_t id;
    char* data;
    std::size_t data_size;
  };

  void _write(TLMAddress addr, std::size_t id, char* data,
        std::size_t data_size);

  void MemoryManager_PortServiceThread();
  void MemoryManagerWriteThread(std::size_t thread_id);
  std::vector<sc_process_handle> ThreadHandles;
  MTQueue<std::function<void(void)>> write_queue;
  MemoryPool* pool;
  sc_mutex pool_mutex;
};

#endif  // BEHAVIOURAL_MEMORYMANAGER_H_
