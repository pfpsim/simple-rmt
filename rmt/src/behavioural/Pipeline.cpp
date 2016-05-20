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

#include "Pipeline.h"
#include <string>
#include <vector>

Pipeline::Pipeline(sc_module_name nm, pfp::core::PFPObject* parent,
      std::string configfile):PipelineSIM(nm, parent, configfile) {
  /*sc_spawn threads*/
}

void Pipeline::init() {
    init_SIM(); /* Calls the init of sub PE's and CE's */
}

void Pipeline::Pipeline_PortServiceThread() {
}

void Pipeline::PipelineThread(std::size_t thread_id) {
}

void Pipeline::configure(std::vector<MatchStageConfig> config_list) {
  for (int i = 0; i < config_list.size(); i++) {
    match_stage[i]->configure(config_list[i]);
  }

  for (auto &p : parser) {
    p->set_first_table(config_list[0].name);
  }
}
