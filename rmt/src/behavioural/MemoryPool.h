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
 * @file MemoryPool.h
 * Memory pool object
 *
 * Created on: April 11, 2016
 * Author: Eric Tremblay
 */

#ifndef BEHAVIOURAL_MEMORYPOOL_H_
#define BEHAVIOURAL_MEMORYPOOL_H_

#include <iostream>
#include <map>
#include <vector>

typedef std::size_t TLMAddress;

class MemoryPoolBlock {
 public:
  MemoryPoolBlock *next_block;
  TLMAddress addr;  /*!< Address of where the data for this block is found */
};

/**
 * Memory Pool class with virtual address space. It is up to the user to translate the address used by this object to physical addresses and back.
 */
class MemoryPool {
 public:
  MemoryPool(TLMAddress start_addr, int num_blks, int blksize);

  TLMAddress get();

  void put(TLMAddress addr);

  TLMAddress getAddr() const;

 private:
  MemoryPoolBlock *free_list;
  int blksize;
  int num_blks;
  int num_free;
  std::vector<MemoryPoolBlock> memory_pool_block_list;
  std::map<TLMAddress, MemoryPoolBlock*> block_map;
};

#endif  // BEHAVIOURAL_MEMORYPOOL_H_
