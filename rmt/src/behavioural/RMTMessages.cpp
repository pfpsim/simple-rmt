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

#include "RMTMessages.h"
#include <string>
#include "pfpsim/core/PFPObject.h"
#include "ControlPlaneAgent.h"
#include "MatchTable.h"

const std::string module_name_ = "RMTMessage";

int RMTMessage::next_id = 0;

RMTMessage::RMTMessage() : _source(""), _destination(""), msg_id(next_id),
      _size(-1), _type("RMTMessage") {
  next_id++;
}

AcceptPackets::AcceptPackets() : RMTMessage() {
  data_type("AcceptPackets");
}

void AcceptPackets::execute(pfp::core::PFPObject& module) {}

AddTableEntry::AddTableEntry() : RMTMessage() {
  data_type("AddTableEntry");
}

void AddTableEntry::execute(pfp::core::PFPObject& module) {
  bm::entry_handle_t handle;

  npulog(normal, std::cout << "Inserting Table Entry: " << table_name
        << " --> " << match_key_str << std::endl;)
  npulog(profile, std::cout << "Inserting Table Entry: " << table_name
        << " --> " << match_key_str << std::endl;)
  auto p4 = P4::get("rmt");
  p4->lock.write_lock();
  bm::MatchErrorCode rc = p4->mt_add_entry(
        0,
        table_name,  match_key,
        action_name, action_data,
        &handle);
  p4->lock.write_unlock();
  if (rc != bm::MatchErrorCode::SUCCESS) {
    std::cout << " ERROR INSERTING WITH CODE: "
          << static_cast<int>(rc) << std::endl;
  } else {
    MatchTable& match_table = static_cast<MatchTable&>(module);

    npulog(profile, std::cout << "Sending TableEntryAdded message: "
          << table_name << " --> " << match_key_str << std::endl;)
    std::shared_ptr<TableEntryAdded> msg = std::make_shared<TableEntryAdded>();
    msg->table_name = table_name;
    msg->match_key_str = match_key_str;
    msg->action_data_str = action_data_str;
    msg->action_name = action_name;
    msg->handle = handle;
    match_table.table_out->put(msg);
  }
}

TableEntryAdded::TableEntryAdded() : RMTMessage() {
  data_type("TableEntryAdded");
}

void TableEntryAdded::execute(pfp::core::PFPObject& module) {}

ModifyTableEntry::ModifyTableEntry() : RMTMessage() {
  data_type("ModifyTableEntry");
}

void ModifyTableEntry::execute(pfp::core::PFPObject& module) {
  npulog(normal, std::cout << "Modifying Table Entry: " << table_name
        << " with handle " << handle << std::endl;)
  npulog(profile, std::cout << "Modifying Table Entry: " << table_name
        << " with handle " << handle << std::endl;)
  auto p4 = P4::get("rmt");
  p4->lock.write_lock();
  bm::MatchErrorCode rc = p4->mt_modify_entry(0, table_name, handle,
        action_name, action_data);
  p4->lock.write_unlock();
  if (rc != bm::MatchErrorCode::SUCCESS) {
    std::cout << " ERROR MODIFYING WITH CODE: "
          << static_cast<int>(rc) << std::endl;
  } else {
    MatchTable& match_table = static_cast<MatchTable&>(module);

    npulog(profile, std::cout << "Sending TableEntryModifed message: "
          << table_name << "Handle: " << handle << std::endl;)
    std::shared_ptr<TableEntryModified> msg
            = std::make_shared<TableEntryModified>();
    msg->table_name = table_name;
    msg->action_data = action_data_str;
    msg->action_name = action_name;
    msg->handle = handle;
    match_table.table_out->put(msg);
  }
}

TableEntryModified::TableEntryModified() : RMTMessage() {
  data_type("TableEntryModified");
}

void TableEntryModified::execute(pfp::core::PFPObject& module) {}

DeleteTableEntry::DeleteTableEntry() : RMTMessage() {
  data_type("DeleteTableEntry");
}

void DeleteTableEntry::execute(pfp::core::PFPObject& module) {
  npulog(normal, std::cout << "Deleting Table Entry: " << table_name
        << " with handle " << handle << std::endl;)
  npulog(profile, std::cout << "Deleting Table Entry: " << table_name
        << " with handle " << handle << std::endl;)
  auto p4 = P4::get("rmt");
  p4->lock.write_lock();
  bm::MatchErrorCode rc = p4->mt_delete_entry(0, table_name, handle);
  p4->lock.write_unlock();
  if (rc != bm::MatchErrorCode::SUCCESS) {
    std::cout << " ERROR MODIFYING WITH CODE: "
          << static_cast<int>(rc) << std::endl;
  } else {
    MatchTable& match_table = static_cast<MatchTable&>(module);

    npulog(profile, std::cout << "Sending TableEntryDeleted message: "
          << table_name << "Handle: " << handle << std::endl;)
    std::shared_ptr<TableEntryDeleted> msg
            = std::make_shared<TableEntryDeleted>();
    msg->table_name = table_name;
    msg->handle = handle;
    match_table.table_out->put(msg);
  }
}

TableEntryDeleted::TableEntryDeleted() : RMTMessage() {
  data_type("TableEntryDeleted");
}

void TableEntryDeleted::execute(pfp::core::PFPObject& module) {}
