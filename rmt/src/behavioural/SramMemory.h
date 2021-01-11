#ifndef BEHAVIOURAL_SRAMMEMORY_H_
#define BEHAVIOURAL_SRAMMEMORY_H_
#include <string>
#include <vector>
#include "../structural/SramMemorySIM.h"
#include "SramMemI.h"

const std::string module_name = "SramMemory";

// #define debug_tlm_mem_transaction 1
template<typename T>
class SramMemory:
public SramMemI<T>,
public SramMemorySIM {
 public:
  /* CE Consturctor */
  explicit SramMemory(sc_module_name nm, pfp::core::PFPObject* parent = 0,
        std::string configfile = "");
  /* User Implementation of the Virtual Functions in the Interface.h */

  /* User Implementation of the Virtual Functions in the MemI.h file */

  virtual void write(SramActionBase* action, unsigned int addr) {
    if (addr < SIZE) {
      wait(WR_LATENCY);
      // npulog(profile, std::cout << "Writing to MEM @ addr " << std::endl;)
      mem[addr] = action;
    } else {
      SC_REPORT_ERROR("Sram Memory Write", "Out of address range!");
    }
  }

  virtual SramActionBase* read(unsigned int addr) {
    if (addr < SIZE) {
      wait(RD_LATENCY);
      return mem[addr];
    } else {
      SC_REPORT_ERROR("Sram Memory Read", "Out of address range!");
     }
  }

 private:
  std::string modulename;
  SramActionBase** mem;
  uint64_t SIZE;
  sc_time RD_LATENCY;
  sc_time WR_LATENCY;
};

/*
	SramMemory Consturctor
 */
// TODO(prerit) add config file for memory --> "TSE_Mem_Config.cfg"
template<typename T>
SramMemory<T>::SramMemory(sc_module_name nm, pfp::core::PFPObject* parent,
      std::string configfile):SramMemorySIM(nm, parent, configfile) {
  RD_LATENCY = sc_time(10, SC_NS);
  WR_LATENCY = sc_time(20, SC_NS);
  SIZE = 1024;
  mem = new SramActionBase*[SIZE];  // allocate the block on host mem in heap
}

#endif  // BEHAVIOURAL_SRAMMEMORY_H_
