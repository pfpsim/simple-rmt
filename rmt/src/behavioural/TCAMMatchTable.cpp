#include "./TCAMMatchTable.h"
#include <string>

TCAMMatchTable::TCAMMatchTable(sc_module_name nm, pfp::core::PFPObject* parent,std::string configfile ):TCAMMatchTableSIM(nm,parent,configfile) {  // NOLINT(whitespace/line_length)
  /*sc_spawn threads*/
}

void TCAMMatchTable::init() {
  init_SIM(); /* Calls the init of sub PE's and CE's */
}
void TCAMMatchTable::TCAMMatchTable_PortServiceThread() {
  // Thread function to service input ports.
}
void TCAMMatchTable::TCAMMatchTableThread(std::size_t thread_id) {
  // Thread function for module functionalty
}
