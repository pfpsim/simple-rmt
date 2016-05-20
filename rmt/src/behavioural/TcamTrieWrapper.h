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
*  TcamTrieWrapper.h
*/

#ifndef BEHAVIOURAL_TCAMTRIEWRAPPER_H_
#define BEHAVIOURAL_TCAMTRIEWRAPPER_H_
#include <vector>
#include <string>
#include "tries/src/Trie.h"
#include "TcamSearchEngineS.h"

template <class T>
class TcamTrieWrapper: public Trie<T> {
 public:
  // Constructor
  TcamTrieWrapper();
  TcamTrieWrapper(RoutingTableEntry<T> *iRoutingTable, int iRoutingTableSize,
        T iDefaultAction, int iDefaultActionSize);

  // Destructor
  virtual ~TcamTrieWrapper();

  // Update
  virtual void update(RoutingTableEntry<T> *iRoutingTable,
        int iRoutingTableSize, typename Trie<T>::Action iAction);

  // Lookup
  virtual T exactPrefixMatch(BitString iPrefix) const;
  virtual T longestPrefixMatch(BitString iPrefix) const;

 private:
  void resolve_ports();

  sc_port<TcamSearchEngineS> *tse_port = nullptr;
  sc_object* current_scmodule;
};

template <class T>
TcamTrieWrapper<T>::TcamTrieWrapper() {
  resolve_ports();
}

template<class T>
TcamTrieWrapper<T>::TcamTrieWrapper(RoutingTableEntry<T> *iRoutingTable,
      int iRoutingTableSize, T iDefaultAction, int iDefaultActionSize)
      : TcamTrieWrapper() {
  (*tse_port)->setDefaultAction(new TcamAction<T>(iDefaultAction));
  wait(1, SC_NS);
  RoutingTableEntry<TcamActionBase*> *wTable
        = new RoutingTableEntry<TcamActionBase*>[iRoutingTableSize];
  for (int i = 0; i < iRoutingTableSize; i++) {
    wTable[i].setData(iRoutingTable[i].getData());
    wTable[i].setLength(iRoutingTable[i].getLength());
    wTable[i].setAction(new TcamAction<T>(iRoutingTable[i].getAction()));
    wTable[i].setActionSize(iRoutingTable[i].getActionSize());
  }
  (*tse_port)->insert(wTable, iRoutingTableSize);
  delete [] wTable;
}

template <class T>
TcamTrieWrapper<T>::~TcamTrieWrapper() {}

template <class T>
void TcamTrieWrapper<T>::update(RoutingTableEntry<T> *iRoutingTable,
        int iRoutingTableSize, typename Trie<T>::Action iAction) {
  if (iAction == Trie<T>::Action::Add) {
    RoutingTableEntry<TcamActionBase*> *wTable
          = new RoutingTableEntry<TcamActionBase*>[iRoutingTableSize];
    for (int i = 0; i < iRoutingTableSize; i++) {
      wTable[i].setData(iRoutingTable[i].getData());
      wTable[i].setLength(iRoutingTable[i].getLength());
      wTable[i].setAction(new TcamAction<T>(iRoutingTable[i].getAction()));
      wTable[i].setActionSize(iRoutingTable[i].getActionSize());
    }
    (*tse_port)->insert(wTable, iRoutingTableSize);
    delete [] wTable;
  } else if (iAction == Trie<T>::Action::Remove) {
    for (int i = 0; i < iRoutingTableSize; i++) {
      (*tse_port)->remove(iRoutingTable[i].getData());
    }
  } else if (iAction == Trie<T>::Action::Reconstruct) {
    RoutingTableEntry<TcamActionBase*> *wTable
          = new RoutingTableEntry<TcamActionBase*>[iRoutingTableSize];
    for (int i = 0; i < iRoutingTableSize; i++) {
      wTable[i].setData(iRoutingTable[i].getData());
      wTable[i].setLength(iRoutingTable[i].getLength());
      wTable[i].setAction(new TcamAction<T>(iRoutingTable[i].getAction()));
      wTable[i].setActionSize(iRoutingTable[i].getActionSize());
    }
    (*tse_port)->reconstruct(wTable, iRoutingTableSize);
    delete [] wTable;
  }
}

template <class T>
T TcamTrieWrapper<T>::exactPrefixMatch(BitString iPrefix) const {
  TcamActionBase* action_base = (*tse_port)->exactSearch(iPrefix);
  TcamAction<T>* action = static_cast<TcamAction<T>*>(action_base);
  return action->getAction();
}

template <class T>
T TcamTrieWrapper<T>::longestPrefixMatch(BitString iPrefix) const {
  TcamActionBase* action_base = (*tse_port)->search(iPrefix);
  TcamAction<T>* action = static_cast<TcamAction<T>*>(action_base);
  return action->getAction();
}

template<class T>
void TcamTrieWrapper<T>::resolve_ports() {
  sc_process_handle this_process = sc_get_current_process_handle();
  current_scmodule = this_process.get_parent_object();
  std::vector<sc_object *> children = current_scmodule->get_child_objects();
  sc_port<TcamSearchEngineS> *tse_portt = nullptr;

  for (auto& each_child : children) {
    if (dynamic_cast< sc_port<TcamSearchEngineS> *>(each_child)) {
      tse_portt = dynamic_cast< sc_port<TcamSearchEngineS> *>(each_child);
      std::string portname = tse_portt->basename();
      if (portname.find("port_0") != std::string::npos) {
        tse_port = dynamic_cast< sc_port<TcamSearchEngineS> *>(each_child);
      }
      // npulog(std::cout << "TSE Port: " << tse_portt->kind() << std::endl;)
    }
  }
}

#endif  // BEHAVIOURAL_TCAMTRIEWRAPPER_H_
