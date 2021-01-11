#ifndef BEHAVIOURAL_SRAMMEMI_H_
#define BEHAVIOURAL_SRAMMEMI_H_

#include "systemc.h"
#include "tlm.h"
#include "SramSearchEngineS.h"

using tlm::tlm_tag;

template <typename T>
class SramMemI : public sc_interface {
 public:
  /* User Logic - Virtual Functions for interface go here */
  virtual void write(SramActionBase* action, unsigned int addr) = 0;
  virtual SramActionBase* read(unsigned int addr) = 0;
};

#endif  // BEHAVIOURAL_SRAMMEMI_H_
