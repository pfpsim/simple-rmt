#ifndef BEHAVIOURAL_SRAMTRIEWRAPPER_H_
#define BEHAVIOURAL_SRAMTRIEWRAPPER_H_
#include <vector>
#include <string>
#include "tries/src/Trie.h"
#include "SramSearchEngineS.h"

template <class T>
class SramTrieWrapper: public Trie<T> {
 public:
  // Constructor
  SramTrieWrapper();
  SramTrieWrapper(RoutingTableEntry<T> *iRoutingTable, int iRoutingTableSize,
        T iDefaultAction, int iDefaultActionSize);

  // Destructor
  virtual ~SramTrieWrapper();

  // Update
  virtual void update(RoutingTableEntry<T> *iRoutingTable,
        int iRoutingTableSize, typename Trie<T>::Action iAction);

  // Lookup
  virtual T exactPrefixMatch(BitString iPrefix) const;
  virtual T longestPrefixMatch(BitString iPrefix) const;

 private:
  void resolve_ports();

  sc_port<SramSearchEngineS> *tse_port = nullptr;
  sc_object* current_scmodule;
};

template <class T>
SramTrieWrapper<T>::SramTrieWrapper() {
  resolve_ports();
}

template<class T>
SramTrieWrapper<T>::SramTrieWrapper(RoutingTableEntry<T> *iRoutingTable,
      int iRoutingTableSize, T iDefaultAction, int iDefaultActionSize)
      : SramTrieWrapper() {
  (*tse_port)->setDefaultAction(new SramAction<T>(iDefaultAction));
  wait(1, SC_NS);
  RoutingTableEntry<SramActionBase*> *wTable
        = new RoutingTableEntry<SramActionBase*>[iRoutingTableSize];
  for (int i = 0; i < iRoutingTableSize; i++) {
    wTable[i].setData(iRoutingTable[i].getData());
    wTable[i].setLength(iRoutingTable[i].getLength());
    wTable[i].setAction(new SramAction<T>(iRoutingTable[i].getAction()));
    wTable[i].setActionSize(iRoutingTable[i].getActionSize());
  }
  (*tse_port)->insert(wTable, iRoutingTableSize);
  delete [] wTable;
}

template <class T>
SramTrieWrapper<T>::~SramTrieWrapper() {}

template <class T>
void SramTrieWrapper<T>::update(RoutingTableEntry<T> *iRoutingTable,
        int iRoutingTableSize, typename Trie<T>::Action iAction) {
  if (iAction == Trie<T>::Action::Add) {
    RoutingTableEntry<SramActionBase*> *wTable
          = new RoutingTableEntry<SramActionBase*>[iRoutingTableSize];
    for (int i = 0; i < iRoutingTableSize; i++) {
      wTable[i].setData(iRoutingTable[i].getData());
      wTable[i].setLength(iRoutingTable[i].getLength());
      wTable[i].setAction(new SramAction<T>(iRoutingTable[i].getAction()));
      wTable[i].setActionSize(iRoutingTable[i].getActionSize());
    }
    (*tse_port)->insert(wTable, iRoutingTableSize);
    delete [] wTable;
  } else if (iAction == Trie<T>::Action::Remove) {
    for (int i = 0; i < iRoutingTableSize; i++) {
      (*tse_port)->remove(iRoutingTable[i].getData());
    }
  } else if (iAction == Trie<T>::Action::Reconstruct) {
    RoutingTableEntry<SramActionBase*> *wTable
          = new RoutingTableEntry<SramActionBase*>[iRoutingTableSize];
    for (int i = 0; i < iRoutingTableSize; i++) {
      wTable[i].setData(iRoutingTable[i].getData());
      wTable[i].setLength(iRoutingTable[i].getLength());
      wTable[i].setAction(new SramAction<T>(iRoutingTable[i].getAction()));
      wTable[i].setActionSize(iRoutingTable[i].getActionSize());
    }
    (*tse_port)->reconstruct(wTable, iRoutingTableSize);
    delete [] wTable;
  }
}

template <class T>
T SramTrieWrapper<T>::exactPrefixMatch(BitString iPrefix) const {
  SramActionBase* action_base = (*tse_port)->exactSearch(iPrefix);
  SramAction<T>* action = static_cast<SramAction<T>*>(action_base);
  return action->getAction();
}

template <class T>
T SramTrieWrapper<T>::longestPrefixMatch(BitString iPrefix) const {
  SramActionBase* action_base = (*tse_port)->search(iPrefix);
  SramAction<T>* action = static_cast<SramAction<T>*>(action_base);
  return action->getAction();
}

template<class T>
void SramTrieWrapper<T>::resolve_ports() {
  sc_process_handle this_process = sc_get_current_process_handle();
  current_scmodule = this_process.get_parent_object();
  std::vector<sc_object *> children = current_scmodule->get_child_objects();
  sc_port<SramSearchEngineS> *tse_portt = nullptr;

  for (auto& each_child : children) {
    if (dynamic_cast< sc_port<SramSearchEngineS> *>(each_child)) {
      tse_portt = dynamic_cast< sc_port<SramSearchEngineS> *>(each_child);
      std::string portname = tse_portt->basename();
      if (portname.find("port_0") != std::string::npos) {
        tse_port = dynamic_cast< sc_port<SramSearchEngineS> *>(each_child);
      }
      // npulog(std::cout << "TSE Port: " << tse_portt->kind() << std::endl;)
    }
  }
}

#endif  // BEHAVIOURAL_SRAMTRIEWRAPPER_H_
