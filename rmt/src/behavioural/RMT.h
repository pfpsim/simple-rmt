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

#ifndef BEHAVIOURAL_RMT_H_
#define BEHAVIOURAL_RMT_H_
#include <vector>
#include <string>
#include "structural/RMTSIM.h"
#include "MatchStageConfig.h"

class RMT: public RMTSIM {
 public:
  SC_HAS_PROCESS(RMT);
  /*Constructor*/
  explicit RMT(sc_module_name nm, pfp::core::PFPObject* parent = 0,
        std::string configfile = "");
  /*Destructor*/
  virtual ~RMT() = default;

 public:
  void init();
  // For ControlPlaneAgentS
  std::shared_ptr<pfp::cp::CommandResult> send_command(
        const std::shared_ptr<pfp::cp::Command> & cmd) override;

 private:
  void RMT_PortServiceThread();
  void RMTThread(std::size_t thread_id);
  std::vector<sc_process_handle> ThreadHandles;

  std::vector<MatchStageConfig> ingress_match_stages;
  std::vector<MatchStageConfig> egress_match_stages;

  // parses the P4 json file (p4 command line arg),
  // creates MatchStageConfig objects
  // determines the toplogical ordering of the tables
  // from the P4 graph and configures the match stages
  void configure_match_stages(std::string p4_json_path);
};

#endif  // BEHAVIOURAL_RMT_H_
