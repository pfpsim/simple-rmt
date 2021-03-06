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

#include "Multiplexer.h"
#include <string>

Multiplexer::Multiplexer(sc_module_name nm , int mux_outputPortSize,
      int mux_inputPortSize, pfp::core::PFPObject* parent,
      std::string configfile)
      :MultiplexerSIM(nm, mux_outputPortSize, mux_inputPortSize, parent,
          configfile), next_write_port(0) {
  /*sc_spawn threads*/
  ThreadHandles.push_back(sc_spawn(sc_bind(&Multiplexer::MultiplexerThread,
        this, 0)));
}

void Multiplexer::init() {
    init_SIM(); /* Calls the init of sub PE's and CE's */
}

void Multiplexer::Multiplexer_PortServiceThread() {
}

void Multiplexer::MultiplexerThread(std::size_t thread_id) {
  while (1) {
    if (!nb_can_gets(mux_input)) {
      wait(multiport_data_written_event(mux_input));
    } else {
      for (auto in = mux_input.begin(); in != mux_input.end(); in++) {
        if ((*in)->nb_can_get()) {
          auto obj = (*in)->get();
          bool not_put = true;
          while (not_put) {
            if (mux_output[next_write_port]->nb_can_put()) {
              wait(1, SC_NS);
              mux_output[next_write_port++]->put(obj);
              not_put = false;
            } else {
              next_write_port++;
            }
            if (next_write_port == mux_output.size()) {
              next_write_port = 0;
            }
          }
        }
      }
    }
  }
}
