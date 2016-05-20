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

#ifndef BEHAVIOURAL_RMTMESSAGES_H_
#define BEHAVIOURAL_RMTMESSAGES_H_
#include <vector>
#include <string>
#include "common/P4.h"
#include "pfpsim/pfpsim.h"

class RMTMessage: public pfp::core::TrType {
 public:
  RMTMessage();

  virtual void execute(pfp::core::PFPObject& module) = 0;

  // TrType Member Functions
  virtual void source(const std::string& src) {_source = src;}
  virtual const std::string& source()const {return _source;}

  virtual void destination(const std::string& dst) {_destination = dst;}
  virtual const std::string& destination() const {return _destination;}

  virtual void id(std::size_t id) {msg_id = id;}
  virtual std::size_t id() const {return msg_id;}

  virtual void size(std::size_t size) {this->_size = size;}
  virtual std::size_t size() const {return _size;}

  virtual void data_type(std::string size) {_type = size;}
  virtual std::string data_type() const {return _type;}

  std::string table_name;

 private:
  std::string _source;
  std::string _destination;
  std::size_t msg_id;
  std::size_t _size;
  std::string _type;

  static int next_id;
};

class AcceptPackets: public RMTMessage {
 public:
  AcceptPackets();

  virtual void execute(pfp::core::PFPObject& module);
};

class AddTableEntry: public RMTMessage {
 public:
  AddTableEntry();

  virtual void execute(pfp::core::PFPObject& module);

  std::vector<bm::MatchKeyParam> match_key;
  std::string action_name;
  bm::ActionData action_data;
  std::string match_key_str;
  std::vector<std::string> action_data_str;
};

class TableEntryAdded: public RMTMessage {
 public:
  TableEntryAdded();

  virtual void execute(pfp::core::PFPObject& module);

  std::vector<bm::MatchKeyParam> match_key;
  std::string action_name;
  std::string match_key_str;
  std::vector<std::string> action_data_str;
  bm::entry_handle_t handle;
};

class ModifyTableEntry: public RMTMessage {
 public:
  ModifyTableEntry();

  virtual void execute(pfp::core::PFPObject& module);

  bm::entry_handle_t handle;
  std::string action_name;
  bm::ActionData action_data;
  std::vector<std::string> action_data_str;
};

class TableEntryModified: public RMTMessage {
 public:
  TableEntryModified();

  virtual void execute(pfp::core::PFPObject& module);

  bm::entry_handle_t handle;
  std::string action_name;
  std::vector<std::string> action_data;
};

class DeleteTableEntry: public RMTMessage {
 public:
  DeleteTableEntry();

  virtual void execute(pfp::core::PFPObject& module);

  bm::entry_handle_t handle;
};

class TableEntryDeleted: public RMTMessage {
 public:
  TableEntryDeleted();

  virtual void execute(pfp::core::PFPObject& module);

  bm::entry_handle_t handle;
};

#endif  // BEHAVIOURAL_RMTMESSAGES_H_
