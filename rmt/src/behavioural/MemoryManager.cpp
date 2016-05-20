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

#include "MemoryManager.h"
#include <string>
#include "MemoryPool.h"
#include "MemoryValue.h"

MemoryManager::MemoryManager(sc_module_name nm , int memPortSize,
      pfp::core::PFPObject* parent, std::string configfile)
      :MemoryManagerSIM(nm, memPortSize, parent, configfile) {
  /*sc_spawn threads*/
  ThreadHandles.push_back(sc_spawn(sc_bind(
        &MemoryManager::MemoryManagerWriteThread, this, 0)));

  // Create memory pool
  // TODO(eric) Get from configuration file
  pool = new MemoryPool(0, 256, 2048);
}

void MemoryManager::init() {
    init_SIM(); /* Calls the init of sub PE's and CE's */
}
void MemoryManager::MemoryManager_PortServiceThread() {
}

void MemoryManager::MemoryManagerWriteThread(std::size_t thread_id) {
  while (1) {
    auto func = write_queue.pop();
    func();
  }
}

TLMAddress MemoryManager::write(std::size_t id, char* data,
      std::size_t data_size) {
  pool_mutex.lock();
  TLMAddress addr = pool->get();
  pool_mutex.unlock();
  npulog(normal, std::cout << " received write call for packet " << id
        << ". Address: " << addr << std::endl;)
  auto func = std::bind(&MemoryManager::_write, this, addr, id, data,
        data_size);
  write_queue.push(func);
  return addr;
}

void MemoryManager::write(TLMAddress addr, std::size_t id, char* data,
      std::size_t data_size) {
  npulog(normal, std::cout << " received overwrite call for packet " << id
        << ". Address: " << addr << std::endl;)
  auto func = std::bind(&MemoryManager::_write, this, addr, id, data,
        data_size);
  write_queue.push(func);
}

void MemoryManager::_write(TLMAddress addr, std::size_t id, char* data,
      std::size_t data_size) {
  npulog(normal, std::cout << " executing write for packet " << id
        << std::endl;)
  std::shared_ptr<MemoryValue> val
        = std::make_shared<MemoryValue>(id, data, data_size);
  mem_port[0]->write(addr, val);
  npulog(normal, std::cout << " write for packet " << id << " is done."
        << std::endl;)
}

void MemoryManager::read(TLMAddress addr, std::size_t& id_out, char*& data_out,
      std::size_t& data_size_out) {
  npulog(normal, std::cout << " performing read at address " << addr
        << std::endl;)
  std::shared_ptr<MemoryValue> val
        = std::dynamic_pointer_cast<MemoryValue>(mem_port[0]->read(addr));
  if (val) {
    data_out = val->data;
    data_size_out = val->data_size;
    id_out = val->data_id;
  } else {
    data_out = nullptr;
    data_size_out = 0;
    id_out = -1;
  }
}

void MemoryManager::free(TLMAddress addr) {
  npulog(normal, std::cout << " freeing memory pool block at address " << addr
        << std::endl;)
  pool_mutex.lock();
  pool->put(addr);
  pool_mutex.unlock();
}
