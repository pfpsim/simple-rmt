#include "Sram.h"
#include <string>

Sram::Sram(sc_module_name nm,
       pfp::core::PFPObject* parent, std::string configfile)
      :SramSIM(nm, parent, configfile) {
  /*sc_spawn threads*/
  std::string delay_unit;
  int delay_time;
  try {
    sram_height = static_cast<int>(GetParameter("height").get());
    sram_width = static_cast<int>(GetParameter("width").get());
    delay_time = static_cast<int>(GetParameter("delay_time").get());
    delay_unit = GetParameter("delay_unit").get();
  } catch (std::exception& e) {
    SC_REPORT_ERROR("Sram Constructor", "Invalid configuration paramter");
  }
  sc_core::sc_time_unit sc_unit = SC_NS;
  if (delay_unit == "SC_FS") {
    sc_unit = SC_FS;
  } else if (delay_unit == "SC_PS") {
    sc_unit = SC_PS;
  } else if (delay_unit == "SC_NS") {
    sc_unit = SC_NS;
  } else if (delay_unit == "SC_US") {
    sc_unit = SC_US;
  } else if (delay_unit == "SC_MS") {
    sc_unit = SC_MS;
  } else if (delay_unit == "SC_SEC") {
    sc_unit = SC_SEC;
  } else {
    SC_REPORT_ERROR("Sram Constructor",
        "Invalid Sram delay unit! Use SystemC sc_time_unit.");
  }
  sram_delay = sc_time(delay_time, sc_unit);
}

void Sram::init() {
    init_SIM(); /* Calls the init of sub PE's and CE's */
}

void Sram::Sram_PortServiceThread() {
}

void Sram::SramThread(std::size_t thread_id) {
}

#include <bitset>
#define debug_sram_mem_transaction false

/// ========================================
//
//  Search
//
/// ========================================

int Sram::search(BitString prefix) {
  int index = 0;
  for (auto it = sram_entries.begin(); it != sram_entries.end(); it++) {
    if ((*it).getData() == prefix.substr(0, (*it).getData().size())
          && it->getValid() == true) {
        break;
    } else {
        index++;
    }
  }


  if (index == sram_entries.size()) {
    index = -1;
  }
#if debug_sram_mem_transaction
  cout << "      " << Sram_name << " - Sram_transaction = { S" << ", " << prefix
  << " } , result = " << hex << index << " at time " << sc_time_stamp() << endl;
#endif

  wait(sram_delay);
  return index;
}

int Sram::exactSearch(BitString prefix) {
  int index = 0;
  for (auto it = sram_entries.begin(); it != sram_entries.end(); it++) {
    if (it->getData() == prefix && it->getValid() == true) {
      wait(sram_delay);
      return index;
    } else {
      index++;
    }
  }
  return -1;
}

/// ========================================
//
//  Insert (no shift)
//
/// ========================================


void Sram::insert(BitString prefix, unsigned int pos) {
  if (sram_entries.size() + 1 > sram_height) {
      throw "Unable to add entry. SRAM size exceeded";
  } else if (prefix.size() > sram_width) {
      throw "Unable to add entry. SRAM width exceeded";
  }

  sc_time waitTime(0, SC_NS);

  if (pos < sram_entries.size()) {
    auto it = sram_entries.begin();
    advance(it, pos);
    SramEntry entry(prefix);
    (*it) = entry;
    waitTime = sram_delay;
  } else if (pos == sram_entries.size()) {
    SramEntry entry(prefix);
    sram_entries.insert(sram_entries.end(), entry);
    waitTime = sram_delay;
  } else {
    int diff = pos - static_cast<int>(sram_entries.size());
    for (int index = 0; index < diff; index++) {
        SramEntry entry("", false);
        sram_entries.insert(sram_entries.end(), entry);
    }
    SramEntry entry(prefix);
    sram_entries.insert(sram_entries.end(), entry);
    waitTime =(diff + 1)*sram_delay;
  }

#if debug_sram_mem_transaction
  cout << "      " << sram_name << " - sram_transaction = { W" << ", "
  << hex << pos << " } , data = " << dec << prefix << " at time "
  << sc_time_stamp() << " delay = " << sram_delay << endl;
#endif

  wait(waitTime);
}

/// ========================================
//
//  Insert and shift entries down
//
/// ========================================

void Sram::insertAndShift(BitString prefix, unsigned int pos) {
  if (sram_entries.size() + 1 > sram_height) {
    throw "Unable to add entry. SRAM size exceeded";
  } else if (prefix.size() > sram_width) {
    throw "Unable to add entry. SRAM width exceeded";
  }

  sc_time waitTime(0, SC_NS);

  if (pos < sram_entries.size()) {
    auto it = sram_entries.begin();
    advance(it, pos);
    SramEntry entry(prefix);
    sram_entries.insert(it, entry);
    waitTime = (static_cast<int>(sram_entries.size()) - pos)*sram_delay;
    } else if (pos == sram_entries.size()) {
        SramEntry entry(prefix);
        sram_entries.insert(sram_entries.end(), entry);
        waitTime = sram_delay;
    } else {
      int diff = pos - static_cast<int>(sram_entries.size());
      for (int index = 0; index < diff - 1; index++) {
        SramEntry entry("", false);
        sram_entries.insert(sram_entries.end(), entry);
      }
        SramEntry entry(prefix);
        sram_entries.insert(sram_entries.end(), entry);
        waitTime = (diff + 1)*sram_delay;
    }

#if debug_sram_mem_transaction
  cout << "      " << sram_name << " - sram_transaction = { W (+ shift)"
  << ", " << hex << pos << " } , data = " << dec << prefix << " at time "
  << sc_time_stamp() << " delay = " << waitTime << endl;
#endif

  wait(waitTime);
}

/// ========================================
//
//  Remove
//
/// ========================================

void Sram::remove(unsigned int pos) {
  if (pos < sram_entries.size()) {
    auto it = sram_entries.begin();
    advance(it, pos);
    it->setValid(false);
    wait(sram_delay);
    return;
  }
}

/// ========================================
//
//  Remove (and shift entries up)
//
/// ========================================

void Sram::removeAndShift(unsigned int pos) {
  if (pos < sram_entries.size()) {
    auto it = sram_entries.begin();
    advance(it, pos);
    sram_entries.erase(it);
    wait((sram_entries.size() - pos + 1)*sram_delay);
  }
}

/// ========================================
//
//  Read
//
/// ========================================

BitString Sram::read(unsigned int pos) {
  BitString result = "";

  if (pos < sram_entries.size()) {
    auto it = sram_entries.begin();
    advance(it, pos);
    if ((*it).getValid()) {
      result =  (*it).getData();
    }
  }

#if debug_sram_mem_transaction
  cout << "      " << sram_name<< " - sram_transaction = { R, " << hex << pos
            << " } , data = " << result << " at time " << sc_time_stamp()
            << " delay = " << sram_delay << endl;
#endif

  wait(sram_delay);
  return result;
}

/// ========================================
//
//  Get SRAM size in bits
//
/// ========================================

int Sram::getSramSize() {
  return sram_height*sram_width;
}

BitString Sram::DecimalToBinaryString(int64 iDecimal,
      const int iNumOfBits) const {
  BitString wString = std::bitset<64>(iDecimal).to_string();
  BitString wFinalString = wString.substr(wString.size() - iNumOfBits);
  return wFinalString;
}

/// ========================================
//
//  Returns the Sram delay
//
/// ========================================

sc_time Sram::getSramDelay() {
  return sram_delay;
}

/// ========================================
//
//  Functions for SramEntry class
//
/// ========================================


SramEntry::SramEntry(BitString d, bool v): data(d), valid(v) {}

BitString SramEntry::getData() const {
    return data;
}

bool SramEntry::getValid() const {
    return valid;
}


void SramEntry::setData(BitString d) {
    data = d;
}

void SramEntry::setValid(bool v) {
    valid = v;
}
