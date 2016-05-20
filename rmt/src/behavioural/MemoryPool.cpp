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

#include "MemoryPool.h"
#include "systemc.h"

MemoryPool::MemoryPool(TLMAddress start_addr, int num_blks, int blksize)
      : blksize(blksize), num_blks(num_blks), num_free(num_blks),
      memory_pool_block_list(num_blks) {
  memory_pool_block_list[0].addr = start_addr;
  free_list = &memory_pool_block_list[0];
  block_map[start_addr] = &memory_pool_block_list[0];

  for (int i = 0; i < (num_blks - 1); i++) {
    memory_pool_block_list[i].next_block = &memory_pool_block_list[i + 1];
    memory_pool_block_list[i + 1].addr =
          memory_pool_block_list[i].addr + blksize;
    block_map[memory_pool_block_list[i + 1].addr] =
          &memory_pool_block_list[i + 1];
  }
  memory_pool_block_list[num_blks - 1].next_block = nullptr;
}

TLMAddress MemoryPool::getAddr() const {
  return memory_pool_block_list[0].addr;
}

TLMAddress MemoryPool::get() {
  if (num_free > 0) {
    MemoryPoolBlock* blk = free_list;
    free_list = blk->next_block;
    num_free--;
    return blk->addr;
  } else {
    SC_REPORT_ERROR("Memory Pool", "No More free blocks.");
  }
}

void MemoryPool::put(TLMAddress addr) {
  auto blk = block_map.find(addr);
  if (blk != block_map.end()) {
    blk->second->next_block = free_list;
    free_list = blk->second;
    num_free++;
  } else {
    // TODO(eric) throw error
  }
}
