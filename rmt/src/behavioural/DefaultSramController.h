#ifndef BEHAVIOURAL_DEFAULTSRAMCONTROLLER_H_
#define BEHAVIOURAL_DEFAULTSRAMCONTROLLER_H_
#include <vector>
#include <string>
#include "../structural/DefaultSramControllerSIM.h"

class DefaultSramController: public DefaultSramControllerSIM {
 public:
  SC_HAS_PROCESS(DefaultSramController);
  /*Constructor*/
  explicit DefaultSramController
      (sc_module_name nm, pfp::core::PFPObject* parent = 0,
      std::string configfile = "");
  /*Destructor*/
  virtual ~DefaultSramController() = default;

 public:
  void init();

  // Insert
  virtual void insert(BitString prefix, SramActionBase* action);
  virtual void insert(RoutingTableEntry<SramActionBase*> *routingTable,
        unsigned int routingTableSize);

  // Remove
  virtual void remove(BitString prefix);

  // Reconstruct
  virtual void reconstruct(RoutingTableEntry<SramActionBase*> *routingTable,
        unsigned int routingTableSize);

  // Search
  virtual SramActionBase* exactSearch(BitString prefix);
  virtual SramActionBase* search(BitString prefix);

  static bool sortEntries(RoutingTableEntry<SramActionBase*> iFirst,
        RoutingTableEntry<SramActionBase*> iSecond);

  virtual void setDefaultAction(SramActionBase* action);

  //  r/w function to read wrtie to mem
  int addrcounter = 0;
  void write_mem(SramActionBase* data_to_allocate, int addr);
  SramActionBase* read_mem(int addr);

 private:
  void DefaultSramController_PortServiceThread();
  void DefaultSramControllerThread(std::size_t thread_id);
  std::vector<sc_process_handle> ThreadHandles;

  int actionTableSize;
  SramActionBase* defaultAction;
};
#endif  // BEHAVIOURAL_DEFAULTSRAMCONTROLLER_H_
