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

#ifndef BEHAVIOURAL_MULTIPLEXER_H_
#define BEHAVIOURAL_MULTIPLEXER_H_
#include <vector>
#include <string>
#include "structural/MultiplexerSIM.h"

class Multiplexer: public MultiplexerSIM {
 public:
  SC_HAS_PROCESS(Multiplexer);
  /*Constructor*/
  Multiplexer(sc_module_name nm , int mux_outputPortSize,
        int mux_inputPortSize , pfp::core::PFPObject* parent = 0,
        std::string configfile = "");
  /*Destructor*/
  virtual ~Multiplexer() = default;

 public:
  void init();

 private:
  void Multiplexer_PortServiceThread();
  void MultiplexerThread(std::size_t thread_id);
  std::vector<sc_process_handle> ThreadHandles;

  int next_write_port;  // next output port to write to
};
#endif  // BEHAVIOURAL_MULTIPLEXER_H_
