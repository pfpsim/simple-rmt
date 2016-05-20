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

/**
* MatchStageConfig.h
*
* Created on: March 9, 2016
* Author: Eric Tremblay
*/
/**
 * @file MatchStageConfig.h
 * Obejct holding necessary information to configure the match stages
 */
#ifndef BEHAVIOURAL_MATCHSTAGECONFIG_H_
#define BEHAVIOURAL_MATCHSTAGECONFIG_H_

#include <vector>
#include <string>

class MatchStageConfig {
 public:
  enum StageType {
    TABLE,
    CONDITIONAL
  };

  StageType stage_type;
  std::string name;
  std::string match_type;
  int max_size;
  std::vector<std::string> next_stages;
  int count = 0;  // used when performing topological sorting
};

#endif  // BEHAVIOURAL_MATCHSTAGECONFIG_H_
