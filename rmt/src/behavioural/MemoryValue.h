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
 * @file MemoryValue.h
 * Wrapper around data and data size in order to have a TrType.
 *
 * Created on: April 12th, 2016
 * Author: Eric Tremblay
 */

#ifndef BEHAVIOURAL_MEMORYVALUE_H_
#define BEHAVIOURAL_MEMORYVALUE_H_

#include <string>
#include "pfpsim/core/TrType.h"

class MemoryValue : public pfp::core::TrType {
 public:
  MemoryValue() {}
  MemoryValue(std::size_t id, char* data, std::size_t data_size)
        : data(data), data_size(data_size), data_id(id) {}

  // TrType Functions
  virtual void source(const std::string& src) {}
  virtual const std::string& source()const {}

  virtual void destination(const std::string& dst) {}
  virtual const std::string& destination() const {}

  virtual void id(std::size_t id) {data_id = id;}
  virtual std::size_t id() const {return data_id;}

  virtual void size(std::size_t size) {data_size = size;}
  virtual std::size_t size() const {return data_size;}

  virtual void data_type(std::string size) {}
  virtual std::string data_type() const {return "MemoryValue";}

  char* data;
  std::size_t data_size;
  std::size_t data_id;
};

#endif  // BEHAVIOURAL_MEMORYVALUE_H_
