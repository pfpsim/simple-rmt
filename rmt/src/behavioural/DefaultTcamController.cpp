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

#include "DefaultTcamController.h"
#define debug_tlm_mem_transaction true
#include <bitset>
#include <algorithm>
#include <string>

DefaultTcamController::DefaultTcamController(sc_module_name nm,
      pfp::core::PFPObject* parent, std::string configfile)
      :DefaultTcamControllerSIM(nm, parent, configfile) {
  /*sc_spawn threads*/
}

void DefaultTcamController::init() {
    init_SIM(); /* Calls the init of sub PE's and CE's */
}
void DefaultTcamController::DefaultTcamController_PortServiceThread() {
}

void DefaultTcamController::DefaultTcamControllerThread(std::size_t thread_id) {
}

/// ================================
//
//  Insert
//
/// ================================

void DefaultTcamController::insert(BitString prefix, TcamActionBase* action) {
    int pos = 0;

    for (; pos < actionTableSize; pos++) {
        BitString temp = tcam_port->read(pos);
        if (temp == prefix) {
            tcam_port->insert(prefix, pos);
            write_mem(action, pos);
            return;
        }
        if (temp.size() < prefix.size()) {
            break;
        }
    }

    if (pos < actionTableSize) {
        for (int i = actionTableSize - 1; i >= pos; i--) {
            // TODO(eric) Don't hardcode the size!!!!!
            if (i < 1024) {
                TcamActionBase* val = read_mem(i);
                write_mem(val, i+1);
            } else {
              cout << "The Address was invalid in insert and shift" << endl;
            }
        }
        actionTableSize++;
    } else {
        actionTableSize += pos - actionTableSize + 1;
    }

    write_mem(action, pos);
    tcam_port->insertAndShift(prefix, pos);
}


void DefaultTcamController::insert(
      RoutingTableEntry<TcamActionBase*> *routingTable,
      unsigned int routingTableSize) {
    if (actionTableSize == 0) {
        reconstruct(routingTable, routingTableSize);
    } else {
        for (int i = 0; i < routingTableSize; i++) {
            insert(routingTable[i].getData(), routingTable[i].getAction());
        }
    }
}

/// ================================
//
//  Reconstruct
//
/// ================================

void DefaultTcamController::reconstruct(
      RoutingTableEntry<TcamActionBase*> *routingTable,
      unsigned int routingTableSize) {
    std::sort(routingTable, routingTable + routingTableSize, sortEntries);

    for (int i = 0; i < routingTableSize; i++) {
        tcam_port->insert(routingTable[i].getData(), i);
        write_mem(routingTable[i].getAction(), i);
    }

    for (int j = routingTableSize; j < actionTableSize; j++) {
        tcam_port->remove(j);
    }

    actionTableSize = routingTableSize;
}

/// ================================
//
//  Remove
//
/// ================================

void DefaultTcamController::remove(BitString prefix) {
    int pos = tcam_port->search(prefix);
    if (pos != -1) {
        tcam_port->removeAndShift(pos);

        for (int i = pos; i < actionTableSize - 1; i++) {
            TcamActionBase* val = read_mem(i + 1);
            write_mem(val, i);
        }

        actionTableSize--;
    }
}

/// ================================
//
//  Search
//
/// ================================

TcamActionBase* DefaultTcamController::exactSearch(BitString prefix) {
  int pos = tcam_port->exactSearch(prefix);
  if (pos == -1) {
    return defaultAction;
  } else {
    return read_mem(pos);
  }
}

TcamActionBase* DefaultTcamController::search(BitString prefix) {
    int pos = tcam_port->search(prefix);
    if (pos == -1) {
      return defaultAction;
    } else {
      TcamActionBase* action = read_mem(pos);
      return action;
    }
}

/// ================================
//
//  Sort Entries by Length
//
/// ================================

bool DefaultTcamController::sortEntries(
      RoutingTableEntry<TcamActionBase*> first,
      RoutingTableEntry<TcamActionBase*> second) {
    if (first.getLength() > second.getLength()) {
        return true;
    } else {
        return false;
    }
}

void DefaultTcamController::setDefaultAction(TcamActionBase* action) {
  defaultAction = action;
}

/*
 * ------------------- MEM R/W Functions
 *
 */

void DefaultTcamController::write_mem(TcamActionBase* data_to_allocate,
      int addr) {
  mem->write(data_to_allocate, addr);
}


TcamActionBase* DefaultTcamController::read_mem(int addr) {
  return mem->read(addr);
}
