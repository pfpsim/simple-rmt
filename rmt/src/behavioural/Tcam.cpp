/*
 * simple-rmt: Example RMT simulation model using the PFPSim Framework
 *
 * Copyright (C) 2016 Concordia Univ., Montreal
 *     Samar Abdi
 *     Umair Aftab
 *     Gordon Bailey
 *     Faras Dewal
 *     Shafigh Parsazad
 *     Eric Tremblay
 *
 * Copyright (C) 2016 Ericsson
 *     Bochra Boughzala
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#include "Tcam.h"
#include <string>

Tcam::Tcam(sc_module_name nm,
       pfp::core::PFPObject* parent, std::string configfile)
      :TcamSIM(nm, parent, configfile) {
  /*sc_spawn threads*/
  std::string delay_unit;
  int delay_time;
  try {
    tcam_height = static_cast<int>(GetParameter("height").get());
    tcam_width = static_cast<int>(GetParameter("width").get());
    delay_time = static_cast<int>(GetParameter("delay_time").get());
    delay_unit = GetParameter("delay_unit").get();
  } catch (std::exception& e) {
    SC_REPORT_ERROR("Tcam Constructor", "Invalid configuration paramter");
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
    SC_REPORT_ERROR("Tcam Constructor",
        "Invalid tcam delay unit! Use SystemC sc_time_unit.");
  }
  tcam_delay = sc_time(delay_time, sc_unit);
}

void Tcam::init() {
    init_SIM(); /* Calls the init of sub PE's and CE's */
}

void Tcam::Tcam_PortServiceThread() {
}

void Tcam::TcamThread(std::size_t thread_id) {
}

#include <bitset>
#define debug_tcam_mem_transaction false

/// ========================================
//
//  Search
//
/// ========================================

int Tcam::search(BitString prefix) {
  int index = 0;
  for (auto it = tcam_entries.begin(); it != tcam_entries.end(); it++) {
    if ((*it).getData() == prefix.substr(0, (*it).getData().size())
          && it->getValid() == true) {
        break;
    } else {
        index++;
    }
  }


  if (index == tcam_entries.size()) {
    index = -1;
  }
#if debug_tcam_mem_transaction
  cout << "      " << tcam_name << " - tcam_transaction = { S" << ", " << prefix
  << " } , result = " << hex << index << " at time " << sc_time_stamp() << endl;
#endif

  wait(tcam_delay);
  return index;
}

int Tcam::exactSearch(BitString prefix) {
  int index = 0;
  for (auto it = tcam_entries.begin(); it != tcam_entries.end(); it++) {
    if (it->getData() == prefix && it->getValid() == true) {
      wait(tcam_delay);
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


void Tcam::insert(BitString prefix, unsigned int pos) {
  if (tcam_entries.size() + 1 > tcam_height) {
      throw "Unable to add entry. TCAM size exceeded";
  } else if (prefix.size() > tcam_width) {
      throw "Unable to add entry. TCAM width exceeded";
  }

  sc_time waitTime(0, SC_NS);

  if (pos < tcam_entries.size()) {
    auto it = tcam_entries.begin();
    advance(it, pos);
    TcamEntry entry(prefix);
    (*it) = entry;
    waitTime = tcam_delay;
  } else if (pos == tcam_entries.size()) {
    TcamEntry entry(prefix);
    tcam_entries.insert(tcam_entries.end(), entry);
    waitTime = tcam_delay;
  } else {
    int diff = pos - static_cast<int>(tcam_entries.size());
    for (int index = 0; index < diff; index++) {
        TcamEntry entry("", false);
        tcam_entries.insert(tcam_entries.end(), entry);
    }
    TcamEntry entry(prefix);
    tcam_entries.insert(tcam_entries.end(), entry);
    waitTime =(diff + 1)*tcam_delay;
  }

#if debug_tcam_mem_transaction
  cout << "      " << tcam_name << " - tcam_transaction = { W" << ", "
  << hex << pos << " } , data = " << dec << prefix << " at time "
  << sc_time_stamp() << " delay = " << tcam_delay << endl;
#endif

  wait(waitTime);
}

/// ========================================
//
//  Insert and shift entries down
//
/// ========================================

void Tcam::insertAndShift(BitString prefix, unsigned int pos) {
  if (tcam_entries.size() + 1 > tcam_height) {
    throw "Unable to add entry. TCAM size exceeded";
  } else if (prefix.size() > tcam_width) {
    throw "Unable to add entry. TCAM width exceeded";
  }

  sc_time waitTime(0, SC_NS);

  if (pos < tcam_entries.size()) {
    auto it = tcam_entries.begin();
    advance(it, pos);
    TcamEntry entry(prefix);
    tcam_entries.insert(it, entry);
    waitTime = (static_cast<int>(tcam_entries.size()) - pos)*tcam_delay;
    } else if (pos == tcam_entries.size()) {
        TcamEntry entry(prefix);
        tcam_entries.insert(tcam_entries.end(), entry);
        waitTime = tcam_delay;
    } else {
      int diff = pos - static_cast<int>(tcam_entries.size());
      for (int index = 0; index < diff - 1; index++) {
        TcamEntry entry("", false);
        tcam_entries.insert(tcam_entries.end(), entry);
      }
        TcamEntry entry(prefix);
        tcam_entries.insert(tcam_entries.end(), entry);
        waitTime = (diff + 1)*tcam_delay;
    }

#if debug_tcam_mem_transaction
  cout << "      " << tcam_name << " - tcam_transaction = { W (+ shift)"
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

void Tcam::remove(unsigned int pos) {
  if (pos < tcam_entries.size()) {
    auto it = tcam_entries.begin();
    advance(it, pos);
    it->setValid(false);
    wait(tcam_delay);
    return;
  }
}

/// ========================================
//
//  Remove (and shift entries up)
//
/// ========================================

void Tcam::removeAndShift(unsigned int pos) {
  if (pos < tcam_entries.size()) {
    auto it = tcam_entries.begin();
    advance(it, pos);
    tcam_entries.erase(it);
    wait((tcam_entries.size() - pos + 1)*tcam_delay);
  }
}

/// ========================================
//
//  Read
//
/// ========================================

BitString Tcam::read(unsigned int pos) {
  BitString result = "";

  if (pos < tcam_entries.size()) {
    auto it = tcam_entries.begin();
    advance(it, pos);
    if ((*it).getValid()) {
      result =  (*it).getData();
    }
  }

#if debug_tcam_mem_transaction
  cout << "      " << tcam_name<< " - tcam_transaction = { R, " << hex << pos
            << " } , data = " << result << " at time " << sc_time_stamp()
            << " delay = " << tcam_delay << endl;
#endif

  wait(tcam_delay);
  return result;
}

/// ========================================
//
//  Get TCAM size in bits
//
/// ========================================

int Tcam::getTcamSize() {
  return tcam_height*tcam_width;
}

BitString Tcam::DecimalToBinaryString(int64 iDecimal,
      const int iNumOfBits) const {
  BitString wString = std::bitset<64>(iDecimal).to_string();
  BitString wFinalString = wString.substr(wString.size() - iNumOfBits);
  return wFinalString;
}

/// ========================================
//
//  Returns the TCAM delay
//
/// ========================================

sc_time Tcam::getTcamDelay() {
  return tcam_delay;
}

/// ========================================
//
//  Functions for TcamEntry class
//
/// ========================================


TcamEntry::TcamEntry(BitString d, bool v): data(d), valid(v) {}

BitString TcamEntry::getData() const {
    return data;
}

bool TcamEntry::getValid() const {
    return valid;
}


void TcamEntry::setData(BitString d) {
    data = d;
}

void TcamEntry::setValid(bool v) {
    valid = v;
}
