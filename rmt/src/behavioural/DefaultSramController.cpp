#include "DefaultSramController.h"
#define debug_tlm_mem_transaction true
#include <bitset>
#include <algorithm>
#include <string>

DefaultSramController::DefaultSramController(sc_module_name nm,
      pfp::core::PFPObject* parent, std::string configfile)
      :DefaultSramControllerSIM(nm, parent, configfile) {
  /*sc_spawn threads*/
}

void DefaultSramController::init() {
    init_SIM(); /* Calls the init of sub PE's and CE's */
}
void DefaultSramController::DefaultSramController_PortServiceThread() {
}

void DefaultSramController::DefaultSramControllerThread(std::size_t thread_id) {
}

/// ================================
//
//  Insert
//
/// ================================

void DefaultSramController::insert(BitString prefix, SramActionBase* action) {
    int pos = 0;

    for (; pos < actionTableSize; pos++) {
        BitString temp = sram_port->read(pos);
        if (temp == prefix) {
            sram_port->insert(prefix, pos);
            write_mem(action, pos);
            return;
        }
        if (temp.size() < prefix.size()) {
            break;
        }
    }

    if (pos < actionTableSize) {
        for (int i = actionTableSize - 1; i >= pos; i--) {
            // TODO(prerit) Don't hardcode the size!!!!!
            if (i < 1024) {
                SramActionBase* val = read_mem(i);
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
    sram_port->insertAndShift(prefix, pos);
}


void DefaultSramController::insert(
      RoutingTableEntry<SramActionBase*> *routingTable,
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

void DefaultSramController::reconstruct(
      RoutingTableEntry<SramActionBase*> *routingTable,
      unsigned int routingTableSize) {
    std::sort(routingTable, routingTable + routingTableSize, sortEntries);

    for (int i = 0; i < routingTableSize; i++) {
        sram_port->insert(routingTable[i].getData(), i);
        write_mem(routingTable[i].getAction(), i);
    }

    for (int j = routingTableSize; j < actionTableSize; j++) {
        sram_port->remove(j);
    }

    actionTableSize = routingTableSize;
}

/// ================================
//
//  Remove
//
/// ================================

void DefaultSramController::remove(BitString prefix) {
    int pos = sram_port->search(prefix);
    if (pos != -1) {
        sram_port->removeAndShift(pos);

        for (int i = pos; i < actionTableSize - 1; i++) {
            SramActionBase* val = read_mem(i + 1);
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

SramActionBase* DefaultSramController::exactSearch(BitString prefix) {
  int pos = sram_port->exactSearch(prefix);
  if (pos == -1) {
    return defaultAction;
  } else {
    return read_mem(pos);
  }
}

SramActionBase* DefaultSramController::search(BitString prefix) {
    int pos = sram_port->search(prefix);
    if (pos == -1) {
      return defaultAction;
    } else {
      SramActionBase* action = read_mem(pos);
      return action;
    }
}

/// ================================
//
//  Sort Entries by Length
//
/// ================================

bool DefaultSramController::sortEntries(
      RoutingTableEntry<SramActionBase*> first,
      RoutingTableEntry<SramActionBase*> second) {
    if (first.getLength() > second.getLength()) {
        return true;
    } else {
        return false;
    }
}

void DefaultSramController::setDefaultAction(SramActionBase* action) {
  defaultAction = action;
}

/*
 * ------------------- MEM R/W Functions
 *
 */

void DefaultSramController::write_mem(SramActionBase* data_to_allocate,
      int addr) {
  mem->write(data_to_allocate, addr);
}


SramActionBase* DefaultSramController::read_mem(int addr) {
  return mem->read(addr);
}
