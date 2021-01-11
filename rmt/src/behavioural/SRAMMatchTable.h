#ifndef BEHAVIOURAL_SRAMMATCHTABLE_H_
#define BEHAVIOURAL_SRAMMATCHTABLE_H_
#include <string>
#include <vector>
#include "../structural/SRAMMatchTableSIM.h"

class SRAMMatchTable: public SRAMMatchTableSIM {  // NOLINT(whitespace/line_length)
 public:
  SC_HAS_PROCESS(SRAMMatchTable);
  /*Constructor*/
  explicit SRAMMatchTable(sc_module_name nm, pfp::core::PFPObject* parent = 0, std::string configfile = "");  // NOLINT(whitespace/line_length)
  /*Destructor*/
  virtual ~SRAMMatchTable() = default;

 public:
  void init();

 private:
  void SRAMMatchTable_PortServiceThread();
  void SRAMMatchTableThread(std::size_t thread_id);
  std::vector<sc_process_handle> ThreadHandles;
};

#endif  // BEHAVIOURAL_SRAMMATCHTABLE_H_
