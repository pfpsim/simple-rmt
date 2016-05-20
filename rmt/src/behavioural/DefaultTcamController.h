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

#ifndef BEHAVIOURAL_DEFAULTTCAMCONTROLLER_H_
#define BEHAVIOURAL_DEFAULTTCAMCONTROLLER_H_
#include <vector>
#include <string>
#include "../structural/DefaultTcamControllerSIM.h"

class DefaultTcamController: public DefaultTcamControllerSIM {
 public:
  SC_HAS_PROCESS(DefaultTcamController);
  /*Constructor*/
  explicit DefaultTcamController
      (sc_module_name nm, pfp::core::PFPObject* parent = 0,
      std::string configfile = "");
  /*Destructor*/
  virtual ~DefaultTcamController() = default;

 public:
  void init();

  // Insert
  virtual void insert(BitString prefix, TcamActionBase* action);
  virtual void insert(RoutingTableEntry<TcamActionBase*> *routingTable,
        unsigned int routingTableSize);

  // Remove
  virtual void remove(BitString prefix);

  // Reconstruct
  virtual void reconstruct(RoutingTableEntry<TcamActionBase*> *routingTable,
        unsigned int routingTableSize);

  // Search
  virtual TcamActionBase* exactSearch(BitString prefix);
  virtual TcamActionBase* search(BitString prefix);

  static bool sortEntries(RoutingTableEntry<TcamActionBase*> iFirst,
        RoutingTableEntry<TcamActionBase*> iSecond);

  virtual void setDefaultAction(TcamActionBase* action);

  //  r/w function to read wrtie to mem
  int addrcounter = 0;
  void write_mem(TcamActionBase* data_to_allocate, int addr);
  TcamActionBase* read_mem(int addr);

 private:
  void DefaultTcamController_PortServiceThread();
  void DefaultTcamControllerThread(std::size_t thread_id);
  std::vector<sc_process_handle> ThreadHandles;

  int actionTableSize;
  TcamActionBase* defaultAction;
};
#endif  // BEHAVIOURAL_DEFAULTTCAMCONTROLLER_H_
