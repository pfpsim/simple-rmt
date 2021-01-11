#ifndef BEHAVIOURAL_SRAMSEARCHENGINE_H_
#define BEHAVIOURAL_SRAMSEARCHENGINE_H_
#include <vector>
#include <string>
#include "structural/SramSearchEngineSIM.h"

class SramSearchEngine: public SramSearchEngineSIM {
 public:
  SC_HAS_PROCESS(SramSearchEngine);
  /*Constructor*/
  explicit SramSearchEngine(sc_module_name nm,
          pfp::core::PFPObject* parent = 0,
          std::string configfile = "");
  /*Destructor*/
  virtual ~SramSearchEngine() = default;

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

  virtual void setDefaultAction(SramActionBase* action);

 private:
  void SramSearchEngine_PortServiceThread();
  void SramSearchEngineThread(std::size_t thread_id);
  std::vector<sc_process_handle> ThreadHandles;
};

#endif  // BEHAVIOURAL_SRAMSEARCHENGINE_H_
