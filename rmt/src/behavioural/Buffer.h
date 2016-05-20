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

#ifndef BEHAVIOURAL_BUFFER_H_
#define BEHAVIOURAL_BUFFER_H_
#include <vector>
#include <string>
#include "structural/BufferSIM.h"
#include "pfpsim/core/TrType.h"
#include "pfpsim/core/MTQueue.h"

class Buffer: public BufferSIM {
 public:
  SC_HAS_PROCESS(Buffer);
  /*Constructor*/
  explicit Buffer(sc_module_name nm, pfp::core::PFPObject* parent = 0,
        std::string configfile = "");
  /*Destructor*/
  virtual ~Buffer() = default;
 public:
  void init();
 private:
  void Buffer_PortServiceThread();
  void BufferThread(std::size_t thread_id);
  std::vector<sc_process_handle> ThreadHandles;

  //! queue to hold packets as they are read by buffer
  MTQueue<std::shared_ptr<pfp::core::TrType>> buffer_queue;
  bool flag_buffer_empty = true;  //! indicates if the queue is empty
  //! used to wake BufferThread when a new packet is added to the queue
  sc_event condition;
};

#endif  // BEHAVIOURAL_BUFFER_H_
