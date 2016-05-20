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

#include "TcamSearchEngine.h"
#include <vector>
#include <string>

TcamSearchEngine::TcamSearchEngine(sc_module_name nm,
      pfp::core::PFPObject* parent, std::string configfile)
      :TcamSearchEngineSIM(nm, parent, configfile) {
    /*sc_spawn threads*/
}

void TcamSearchEngine::init() {
    init_SIM(); /* Calls the init of sub PE's and CE's */
}

void TcamSearchEngine::TcamSearchEngine_PortServiceThread() {
}

void TcamSearchEngine::TcamSearchEngineThread(std::size_t thread_id) {
}

// Insert
void TcamSearchEngine::insert(BitString prefix, TcamActionBase* action) {
  controller->insert(prefix, action);
}

void TcamSearchEngine::insert(RoutingTableEntry<TcamActionBase*> *routingTable,
      unsigned int routingTableSize) {
  controller->insert(routingTable, routingTableSize);
}

// Remove
void TcamSearchEngine::remove(BitString prefix) {
  controller->remove(prefix);
}

// Reconstruct
void TcamSearchEngine::reconstruct(
      RoutingTableEntry<TcamActionBase*> *routingTable,
      unsigned int routingTableSize) {
  controller->reconstruct(routingTable, routingTableSize);
}

// Search
TcamActionBase* TcamSearchEngine::exactSearch(BitString prefix) {
  return controller->exactSearch(prefix);
}

TcamActionBase* TcamSearchEngine::search(BitString prefix) {
  return controller->search(prefix);
}

void TcamSearchEngine::setDefaultAction(TcamActionBase* action) {
  controller->setDefaultAction(action);
}
