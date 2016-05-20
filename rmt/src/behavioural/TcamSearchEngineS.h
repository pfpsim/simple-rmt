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

#ifndef BEHAVIOURAL_TCAMSEARCHENGINES_H_
#define BEHAVIOURAL_TCAMSEARCHENGINES_H_

#include "systemc.h"
#include "tlm.h"
#include "tries/src/RoutingTableEntry.h"

using tlm::tlm_tag;

class TcamActionBase {};

template<class T>
class TcamAction: public TcamActionBase {
 public:
  TcamAction() {}
  explicit TcamAction(T action) : action(action) {}
  TcamAction(const TcamAction& tcam_action) {
    this->action = tcam_action.getAction();
  }

  // Getter
  T getAction() const {
    return action;
  }

  // Operator Overloading
  TcamAction& operator=(const TcamAction& tcam_action) {
    if (&tcam_action != this) {
      this->action = tcam_action.getAction();
    }
    return *this;
  }

  TcamAction& operator=(const T& action) {
    this->action = action;
    return *this;
  }

 private:
  T action;
};

class TcamSearchEngineS : public sc_interface {
 public:
  /* User Logic - Virtual Functions for interface go here */
  // Insert
  virtual void insert(BitString prefix, TcamActionBase* action) = 0;
  virtual void insert(RoutingTableEntry<TcamActionBase*> *routingTable,
        unsigned int routingTableSize) = 0;

  // Remove
  virtual void remove(BitString prefix) = 0;

  // Reconstruct
  virtual void reconstruct(RoutingTableEntry<TcamActionBase*> *routingTable,
        unsigned int routingTableSize) = 0;

  // Search
  virtual TcamActionBase* exactSearch(BitString prefix) = 0;
  virtual TcamActionBase* search(BitString prefix) = 0;

  virtual void setDefaultAction(TcamActionBase* action) = 0;
};

#endif  // BEHAVIOURAL_TCAMSEARCHENGINES_H_
