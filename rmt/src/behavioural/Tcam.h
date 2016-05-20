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

#ifndef BEHAVIOURAL_TCAM_H_
#define BEHAVIOURAL_TCAM_H_
#include <list>
#include <vector>
#include <string>
#include "structural/TcamSIM.h"

class Tcam: public TcamSIM {
 public:
  SC_HAS_PROCESS(Tcam);
  /*Constructor*/
  Tcam(sc_module_name nm, pfp::core::PFPObject* parent = 0,
        std::string configfile = "");
  /*Destructor*/
  virtual ~Tcam() = default;

 public:
  void init();

  virtual void insert(BitString prefix, unsigned int pos);
  virtual void insertAndShift(BitString prefix, unsigned int pos);

  virtual int exactSearch(BitString prefix);
  virtual int search(BitString prefix);

  virtual void remove(unsigned int pos);
  virtual void removeAndShift(unsigned int pos);

  virtual BitString read(unsigned int pos);

  // Get Size of TCAM in Bits
  int getTcamSize();

  // GET TCAM Delay;
  sc_time getTcamDelay();

 private:
  void Tcam_PortServiceThread();
  void TcamThread(std::size_t thread_id);
  std::vector<sc_process_handle> ThreadHandles;

  std::list<TcamEntry> tcam_entries;  // TCAM entries
  unsigned int tcam_height;  // Number of possible entries in tcam
  unsigned int tcam_width;  // Number of possible bits per entry
  sc_time tcam_delay;

  // Convert Decimal number to a binary string
  BitString DecimalToBinaryString(int64 iDecimal, const int iNumOfBits) const;
};

#endif  // BEHAVIOURAL_TCAM_H_
