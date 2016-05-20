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

#ifndef BEHAVIOURAL_CONTROLPLANE_H_
#define BEHAVIOURAL_CONTROLPLANE_H_
#include <vector>
#include <string>
#include "structural/ControlPlaneSIM.h"
#include "pfpsim/core/debugger/CPDebuggerInterface.h"

using namespace pfp;  // NOLINT(build/namespaces)

class ControlPlane: public ControlPlaneSIM,
      public pfp::core::db::CPDebuggerInterface,
      public cp::CommandProcessor, public cp::ResultProcessor {
 public:
  SC_HAS_PROCESS(ControlPlane);
  /*Constructor*/
  explicit ControlPlane(sc_module_name nm, pfp::core::PFPObject* parent = 0,
        std::string configfile = "");
  /*Destructor*/
  virtual ~ControlPlane() = default;

 public:
  void init();

  // Implementing methods from CPDebuggerInterface
  void do_command(std::string cmd) override;

  // For cp::CommandProcessor
  std::shared_ptr<cp::CommandResult> process(cp::InsertCommand*) override;
  std::shared_ptr<cp::CommandResult> process(cp::ModifyCommand*) override;
  std::shared_ptr<cp::CommandResult> process(cp::DeleteCommand*) override;
  std::shared_ptr<cp::CommandResult> process(cp::BootCompleteCommand*) override;

  // For cp::ResultProcessor
  void process(cp::InsertResult*) override;
  void process(cp::ModifyResult*) override;
  void process(cp::DeleteResult*) override;
  void process(cp::FailedResult*) override;

 private:
  void CPThread(std::size_t thread_id);
  void command_processing_thread();
  void _insert_entry(std::string table_name, std::string match_key,
        std::string action_name, std::vector<std::string> action_data);
  void _delete_entry(std::string table_name, uint64_t handle);
  void _modify_entry(std::string table_name, uint64_t handle,
        std::string action_name, std::vector<std::string> action_data);
  std::vector<sc_process_handle> ThreadHandles;

  MTQueue<std::shared_ptr<cp::Command> > command_queue;
};

#endif  // BEHAVIOURAL_CONTROLPLANE_H_
