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

#include "Buffer.h"
#include <string>

Buffer::Buffer
(sc_module_name nm, pfp::core::PFPObject* parent, std::string configfile)
  : BufferSIM(nm, parent, configfile) {
  /*sc_spawn threads*/
  ThreadHandles.push_back(sc_spawn(sc_bind(&Buffer::Buffer_PortServiceThread,
        this)));
  ThreadHandles.push_back(sc_spawn(sc_bind(&Buffer::BufferThread, this, 0)));
}

void Buffer::init() {
    init_SIM(); /* Calls the init of sub PE's and CE's */
}
void Buffer::Buffer_PortServiceThread() {
  while (1) {
    if (!buffer_in->nb_can_get()) {
      wait(buffer_in->ok_to_get());
    } else {
      // read input
      auto received = buffer_in->get();
      std::string received_type;
      if (received->data_type() == "PacketPointer") {
        received_type = "packet";
      } else {
        received_type = "message";
      }
      npulog(profile, cout << module_name() << " received " << received_type
            << " " << received->id() << endl;)
      npulog(normal, cout << module_name() << " received " << received_type
            << " " << received->id() << endl;)
      // push into queue
      buffer_queue.push(received);
      npulog(profile, cout << module_name() << " pushed " << received_type
            << " " << received->id() << " into queue" << endl;)

      // notify other thread
      flag_buffer_empty = false;
      condition.notify();
    }
  }
}
void Buffer::BufferThread(std::size_t thread_id) {
  while (1) {
    while (flag_buffer_empty) {
      wait(condition);
    }

    // wait
    wait(1, SC_NS);

    // read queue
    auto received = buffer_queue.pop();
    std::string received_type;
    if (received->data_type() == "PacketPointer") {
      received_type = "packet";
    } else {
      received_type = "message";
    }
    npulog(profile, cout << module_name() << " popped " << received_type
          << " " << received->id() << " from queue" << endl;)

    if (!buffer_out->nb_can_put()) {
      wait(buffer_out->ok_to_put());
    }
    // write to output port
    buffer_out->put(received);
    npulog(profile, cout << module_name() << " wrote " << received_type
          << " " << received->id() << endl;)
    npulog(normal, cout << module_name() << " wrote " << received_type
          << " " << received->id() << endl;)
    int size = 0;
    buffer_queue.size(size);
    if (size == 0) {
      flag_buffer_empty = true;
    }
  }
}
