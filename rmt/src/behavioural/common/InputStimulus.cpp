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
 * InputStimulus.cpp
 *
 *  Created on: Feb 7, 2016
 *      Author: samar
 */

#include "InputStimulus.h"
#include <string>

InputStimulus::InputStimulus() {
  // TODO(whoever) Auto-generated constructor stub
}

InputStimulus::~InputStimulus() {
  // TODO(whoever) Auto-generated destructor stub
}

InputStimulus::InputStimulus(std::size_t id, ingress_stimulus_type input) {
  id_ = id;
  packet = input;
}

void InputStimulus::source(const std::string& src) {
  InputStimulus::src = src;
}
const std::string& InputStimulus::source()const {
  return src;
}

void InputStimulus::destination(const std::string& dst) {
  InputStimulus::dest = dst;
}
const std::string& InputStimulus::destination() const {
  return dest;
}

void InputStimulus::id(std::size_t id) {
  id_ = id;
}

std::size_t InputStimulus::id() const {
  return id_;
}

void InputStimulus::size(std::size_t size) {
  size_ = size;
}

std::size_t InputStimulus::size() const {
  return size_;
}

void InputStimulus::data_type(std::string data) {
  type = data;
}

std::string InputStimulus::data_type() const {
  return "InputStimulus";
}

void InputStimulus::command(const std::string& cmd) {
  command_ = cmd;
}
const std::string& InputStimulus::command() const {
  return command_;
}

ingress_stimulus_type InputStimulus::stimulus_object() {
  return packet;
}
