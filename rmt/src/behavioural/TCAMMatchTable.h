#ifndef BEHAVIOURAL_TCAMMATCHTABLE_H_
#define BEHAVIOURAL_TCAMMATCHTABLE_H_
#include <string>
#include <vector>
#include "../structural/TCAMMatchTableSIM.h"

class TCAMMatchTable: public TCAMMatchTableSIM {  // NOLINT(whitespace/line_length)
 public:
  SC_HAS_PROCESS(TCAMMatchTable);
  /*Constructor*/
  explicit TCAMMatchTable(sc_module_name nm, pfp::core::PFPObject* parent = 0, std::string configfile = "");  // NOLINT(whitespace/line_length)
  /*Destructor*/
  virtual ~TCAMMatchTable() = default;

 public:
  void init();

 private:
  void TCAMMatchTable_PortServiceThread();
  void TCAMMatchTableThread(std::size_t thread_id);
  std::vector<sc_process_handle> ThreadHandles;
};

#endif  // BEHAVIOURAL_TCAMMATCHTABLE_H_
