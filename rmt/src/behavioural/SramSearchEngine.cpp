#include "SramSearchEngine.h"
#include <vector>
#include <string>

SramSearchEngine::SramSearchEngine(sc_module_name nm,
      pfp::core::PFPObject* parent, std::string configfile)
      :SramSearchEngineSIM(nm, parent, configfile) {
    /*sc_spawn threads*/
}

void SramSearchEngine::init() {
    init_SIM(); /* Calls the init of sub PE's and CE's */
}

void SramSearchEngine::SramSearchEngine_PortServiceThread() {
}

void SramSearchEngine::SramSearchEngineThread(std::size_t thread_id) {
}

// Insert
void SramSearchEngine::insert(BitString prefix, SramActionBase* action) {
  sram_controller->insert(prefix, action);
}

void SramSearchEngine::insert(RoutingTableEntry<SramActionBase*> *routingTable,
      unsigned int routingTableSize) {
  sram_controller->insert(routingTable, routingTableSize);
}

// Remove
void SramSearchEngine::remove(BitString prefix) {
  sram_controller->remove(prefix);
}

// Reconstruct
void SramSearchEngine::reconstruct(
      RoutingTableEntry<SramActionBase*> *routingTable,
      unsigned int routingTableSize) {
  sram_controller->reconstruct(routingTable, routingTableSize);
}

// Search
SramActionBase* SramSearchEngine::exactSearch(BitString prefix) {
  return sram_controller->exactSearch(prefix);
}

SramActionBase* SramSearchEngine::search(BitString prefix) {
  return sram_controller->search(prefix);
}

void SramSearchEngine::setDefaultAction(SramActionBase* action) {
  sram_controller->setDefaultAction(action);
}
