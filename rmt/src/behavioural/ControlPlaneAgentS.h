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

#ifndef BEHAVIOURAL_CONTROLPLANEAGENTS_H_
#define BEHAVIOURAL_CONTROLPLANEAGENTS_H_

#include "systemc.h"
#include "tlm.h"
using tlm::tlm_tag;

#include "pfpsim/core/cp/Commands.h"

// TODO(gordon) namespace pfp {
// Need to wait for this to be supported in pfpgen

class ControlPlaneAgentS : public sc_interface {
 public:
    virtual std::shared_ptr<pfp::cp::CommandResult>
    send_command(const std::shared_ptr<pfp::cp::Command> & cmd) = 0;
};

// TODO(gordon) } // end namespace pfp
#endif  // BEHAVIOURAL_CONTROLPLANEAGENTS_H_
