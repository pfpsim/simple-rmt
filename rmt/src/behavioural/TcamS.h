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

#ifndef BEHAVIOURAL_TCAMS_H_
#define BEHAVIOURAL_TCAMS_H_

#include "systemc.h"
#include "tlm.h"
#include "tries/src/BitString.h"

using tlm::tlm_tag;

class TcamEntry {
 public:
  explicit TcamEntry(BitString d = "", bool v = true);

  // Getters
  BitString getData() const;
  bool getValid() const;

  // Setters
  void setData(BitString d);
  void setValid(bool v);

 private:
  BitString data;
  bool valid;
};

class TcamS : public sc_interface {
 public:
  /* User Logic - Virtual Functions for interface go here */
  virtual void insert(BitString prefix, unsigned int pos) = 0;
  virtual void insertAndShift(BitString prefix, unsigned int pos) = 0;

  virtual int exactSearch(BitString prefix) = 0;
  virtual int search(BitString prefix) = 0;

  virtual void remove(unsigned int pos) = 0;
  virtual void removeAndShift(unsigned int pos) = 0;

  virtual BitString read(unsigned int pos) = 0;
};

#endif  // BEHAVIOURAL_TCAMS_H_
