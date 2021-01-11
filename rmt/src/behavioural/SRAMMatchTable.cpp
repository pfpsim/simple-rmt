#include "./SRAMMatchTable.h"
#include <string>

SRAMMatchTable::SRAMMatchTable(sc_module_name nm, pfp::core::PFPObject* parent,std::string configfile ):SRAMMatchTableSIM(nm,parent,configfile) {  // NOLINT(whitespace/line_length)
  /*sc_spawn threads*/
}

void SRAMMatchTable::init() {
  init_SIM(); /* Calls the init of sub PE's and CE's */
}
void SRAMMatchTable::SRAMMatchTable_PortServiceThread() {
  // Thread function to service input ports.
}
void SRAMMatchTable::SRAMMatchTableThread(std::size_t thread_id) {
  // Thread function for module functionalty
}
