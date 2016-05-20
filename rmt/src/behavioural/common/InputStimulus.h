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
 * InputStimulus.h
 *
 *  Created on: Feb 7, 2016
 *      Author: samar
 */

#ifndef BEHAVIOURAL_COMMON_INPUTSTIMULUS_H_
#define BEHAVIOURAL_COMMON_INPUTSTIMULUS_H_
#include <tuple>
#include <string>
#include "Packet.h"
#include "pfpsim/core/TrType.h"

typedef std::tuple<Packet, std::size_t, std::size_t> ingress_stimulus_type;

class InputStimulus: public pfp::core::TrType {
 public:
  InputStimulus();
  InputStimulus(std::size_t id, ingress_stimulus_type input);
  InputStimulus(const InputStimulus & other) = default;

  virtual ~InputStimulus();

  void source(const std::string& src);
  const std::string& source() const;

  void destination(const std::string& dst);
  const std::string& destination() const;

  void id(std::size_t id);
  std::size_t id() const;

  void size(std::size_t size);
  std::size_t size() const;

  void data_type(std::string size);
  std::string data_type() const;

  void command(const std::string& cmd);
  const std::string& command() const;

  ingress_stimulus_type stimulus_object();

  inline friend std::ostream& operator<<(std::ostream& out,
        const InputStimulus& v) {
    out << "{" << std::endl;
    out << "}" << std::endl;

    return out;
  }

 private:
  std::string src;
  std::string dest;
  std::string type;
  std::size_t id_;
  std::string command_;
  std::size_t size_;
  ingress_stimulus_type packet;
};

#endif  // BEHAVIOURAL_COMMON_INPUTSTIMULUS_H_
