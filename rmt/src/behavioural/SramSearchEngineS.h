#ifndef BEHAVIOURAL_SRAMSEARCHENGINES_H_
#define BEHAVIOURAL_SRAMSEARCHENGINES_H_

#include "systemc.h"
#include "tlm.h"
#include "tries/src/RoutingTableEntry.h"

using tlm::tlm_tag;

class SramActionBase {};

template<class T>
class SramAction: public SramActionBase {
 public:
  SramAction() {}
  explicit SramAction(T action) : action(action) {}
  SramAction(const SramAction& sram_action) {
    this->action = sram_action.getAction();
  }

  // Getter
  T getAction() const {
    return action;
  }

  // Operator Overloading
  SramAction& operator=(const SramAction& sram_action) {
    if (&sram_action != this) {
      this->action = sram_action.getAction();
    }
    return *this;
  }

  SramAction& operator=(const T& action) {
    this->action = action;
    return *this;
  }

 private:
  T action;
};

class SramSearchEngineS : public sc_interface {
 public:
  /* User Logic - Virtual Functions for interface go here */
  // Insert
  virtual void insert(BitString prefix, SramActionBase* action) = 0;
  virtual void insert(RoutingTableEntry<SramActionBase*> *routingTable,
        unsigned int routingTableSize) = 0;

  // Remove
  virtual void remove(BitString prefix) = 0;

  // Reconstruct
  virtual void reconstruct(RoutingTableEntry<SramActionBase*> *routingTable,
        unsigned int routingTableSize) = 0;

  // Search
  virtual SramActionBase* exactSearch(BitString prefix) = 0;
  virtual SramActionBase* search(BitString prefix) = 0;

  virtual void setDefaultAction(SramActionBase* action) = 0;
};

#endif  // BEHAVIOURAL_SRAMSEARCHENGINES_H_
