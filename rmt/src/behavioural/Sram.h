#ifndef BEHAVIOURAL_SRAM_H_
#define BEHAVIOURAL_SRAM_H_
#include <list>
#include <vector>
#include <string>
#include "structural/SramSIM.h"

class Sram: public SramSIM {
 public:
  SC_HAS_PROCESS(Sram);
  /*Constructor*/
  Sram(sc_module_name nm, pfp::core::PFPObject* parent = 0,
        std::string configfile = "");
  /*Destructor*/
  virtual ~Sram() = default;

 public:
  void init();

  virtual void insert(BitString prefix, unsigned int pos);
  virtual void insertAndShift(BitString prefix, unsigned int pos);

  virtual int exactSearch(BitString prefix);
  virtual int search(BitString prefix);

  virtual void remove(unsigned int pos);
  virtual void removeAndShift(unsigned int pos);

  virtual BitString read(unsigned int pos);

  // Get Size of Sram in Bits
  int getSramSize();

  // GET SRAM Delay;
  sc_time getSramDelay();

 private:
  void Sram_PortServiceThread();
  void SramThread(std::size_t thread_id);
  std::vector<sc_process_handle> ThreadHandles;

  std::list<SramEntry> sram_entries;  // SRAM entries
  unsigned int sram_height;  // Number of possible entries in sram
  unsigned int sram_width;  // Number of possible bits per entry
  sc_time sram_delay;

  // Convert Decimal number to a binary string
  BitString DecimalToBinaryString(int64 iDecimal, const int iNumOfBits) const;
};

#endif  // BEHAVIOURAL_SRAM_H_
