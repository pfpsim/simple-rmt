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

#include "RMT.h"
#include <jsoncpp/json.h>
#include <map>
#include <string>
#include <vector>
#include "common/P4.h"

RMT::RMT(sc_module_name nm,
       pfp::core::PFPObject* parent, std::string configfile)
      :RMTSIM(nm, parent, configfile) {
    /*sc_spawn threads*/
}

void RMT::init() {
    init_SIM(); /* Calls the init of sub PE's and CE's */
    std::string p4_json_path = SPARG("p4");
    configure_match_stages(p4_json_path);
}

void RMT::RMT_PortServiceThread() {
}

void RMT::RMTThread(std::size_t thread_id) {
}

std::shared_ptr<pfp::cp::CommandResult> RMT::send_command(
        const std::shared_ptr<pfp::cp::Command> & cmd) {
  return cp_agent->send_command(cmd);
}

void RMT::configure_match_stages(std::string p4_json_path) {
  ifstream p4_json_file;
  p4_json_file.open(p4_json_path);
  if (p4_json_file.is_open()) {
    Json::Value root;
    p4_json_file >> root;
    // Iterate over every pipeline
    const Json::Value& pipeline_list = root["pipelines"];
    for (auto &pipeline : pipeline_list) {
      const std::string pipeline_name = pipeline["name"].asString();
      const std::string init_table = pipeline["init_table"].asString();
      // Iterate over every table in pipeline
      const Json::Value& table_list = pipeline["tables"];
      std::map<std::string, MatchStageConfig> config_map;
      for (auto &table : table_list) {
        const std::string table_name = table["name"].asString();
        const std::string match_type = table["match_type"].asString();
        const int max_size = table["max_size:"].asInt();
        MatchStageConfig config;
        config.stage_type = MatchStageConfig::TABLE;
        config.name = table_name;
        config.match_type = match_type;
        config.max_size = max_size;
        // Get next tables for each table
        const Json::Value& next_table_list = table["next_tables"];
        const Json::Value& action_list = table["actions"];
        std::vector<std::string> next_tables;
        for (auto &action : action_list) {
          const std::string action_name = action.asString();
          const std::string next_table_name = next_table_list[action_name]
                .asString();
          if (next_table_name != "") {
            bool exists = false;
            for (auto &next_table : next_tables) {
              if (next_table == next_table_name) {
                exists = true;
              }
            }
            if (!exists) {
              next_tables.push_back(next_table_name);
            }
          }
        }
        config.next_stages = next_tables;
        config_map[config.name] = config;
      }
      // Iterate over every conditional
      const Json::Value& conditional_list = pipeline["conditionals"];
      for (auto &conditional : conditional_list) {
        const std::string conditional_name = conditional["name"].asString();
        const std::string true_next = conditional["true_next"].asString();
        const std::string false_next = conditional["false_next"].asString();
        MatchStageConfig config;
        config.stage_type = MatchStageConfig::CONDITIONAL;
        config.name = conditional_name;
        config.max_size = -1;
        config.match_type = "";
        std::vector<std::string> next_tables;
        if (true_next != "") {
          next_tables.push_back(true_next);
        }
        if (false_next != "") {
          next_tables.push_back(false_next);
        }
        config.next_stages = next_tables;
        config_map[config.name] = config;
      }

      // Perform topological ordering using algorithm discussed in
      // http://homes.cs.washington.edu/~jrl/421slides/lec5.pdf
      std::vector<MatchStageConfig> *config_list_ptr;
      if (pipeline_name == "ingress") {
        config_list_ptr = &ingress_match_stages;
      } else if (pipeline_name == "egress") {
        config_list_ptr = &egress_match_stages;
      }
      for (auto &config : config_map) {
        for (auto &next_stage : config.second.next_stages) {
          config_map[next_stage].count++;
        }
      }
      std::vector<MatchStageConfig> nodes_with_no_incoming_edges;
      nodes_with_no_incoming_edges.push_back(config_map[init_table]);
      while (!config_map.empty()) {
        auto curr_config = nodes_with_no_incoming_edges[0];
        nodes_with_no_incoming_edges.erase(
                nodes_with_no_incoming_edges.begin());
        config_list_ptr->push_back(curr_config);
        config_map.erase(curr_config.name);
        for (auto &next_stage : curr_config.next_stages) {
          for (auto &config : config_map) {
            if (config.second.name == next_stage) {
              config.second.count = config.second.count - 1;
              if (config.second.count == 0) {
                nodes_with_no_incoming_edges.push_back(
                        config_map[config.second.name]);
              }
            }
          }
        }
      }
    }
    // log the topology
    p4_json_file.close();
    npulog(debug,
    cout << "TOPOLOGY: ingress:";
    for (int i = 0; i < ingress_match_stages.size(); i++) {
      if (i != 0) {
        cout << "->" << ingress_match_stages[i].name;
      } else {
        cout << ingress_match_stages[i].name;
      }
    }
    cout << endl;)
    npulog(debug,
    cout << "TOPOLOGY: egress:";
    for (int i = 0; i < egress_match_stages.size(); i++) {
      if (i != 0) {
        cout << "->" << egress_match_stages[i].name;
      } else {
        cout << egress_match_stages[i].name;
      }
    }
    cout << endl;)

    // Send configs to pipelines
    ingress_pipeline->configure(ingress_match_stages);
    egress_pipeline->configure(egress_match_stages);

  } else {
    SC_REPORT_ERROR("RMT", "Unable to open p4 json file.");
    sc_stop();
  }
}
