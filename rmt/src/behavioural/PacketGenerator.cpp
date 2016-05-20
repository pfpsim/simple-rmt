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

#include "PacketGenerator.h"
#include <string>

PacketGenerator::PacketGenerator(sc_module_name nm,
      pfp::core::PFPObject* parent,
      std::string configfile):PacketGeneratorSIM(nm, parent, configfile) {
  /*sc_spawn threads*/
  ThreadHandles.push_back(sc_spawn(sc_bind(
        &PacketGenerator::PacketGeneratorThread, this, 0)));
}

void PacketGenerator::init() {
    init_SIM(); /* Calls the init of sub PE's and CE's */
}

void PacketGenerator::PacketGenerator_PortServiceThread() {
}

void PacketGenerator::PacketGeneratorThread(std::size_t thread_id) {
  std::string inputfile = GetParameter("pcap_input").get();
  PcapRepeater pcapRepeater(CONFIGROOT+inputfile);

  auto packet_gen = std::bind(&PcapRepeater::getNext, &pcapRepeater);

  // wait(1000*10*1000, SC_NS);

  auto seed = std::chrono::high_resolution_clock::now().time_since_epoch()
        .count();
  std::mt19937 rng(seed);
  int contexts = 1024;  // TODO(whoever) :[SimulationParameters]
  int isolation_groups = 4;  // TODO(whoever) :[SimulationParameters]
  std::uniform_int_distribution<std::size_t> uid_contexts(0, contexts - 1);
  std::uniform_int_distribution<std::size_t> uid_isolation_groups(0,
        isolation_groups - 1);
  for (std::size_t i = 0; pcapRepeater.hasNext(); i++) {
#ifdef POISSON_ARRIVAL_PROCESS
  /* WHEN USING AN EXPONENTIAL DISTRIBUTION, SUBSCRIPTION_DELAY PARAMETER
    * IS EXPECTED NUMBER OF PACKETS (RANDOM EVENTS) PER TIME UNIT
    * EG SUBSCRIPTION_DELAY = 0.02 = LAMBDA = 0.02 packets per time unit on average
    * EG SUBSCRIPTION_DELAY = 5.00 = LAMBDA = 5 packets per time unit on average
    * THEN EXPONENTIAL DISTRIBUTION GIVES TIME TO NEXT RANDOM EVENT, IE THE DELAY
    */
    int subscriptiondelay = SimulationParameters["subscription_delay"].get();
    std::exponential_distribution<double> ed(subscriptiondelay);
    double wait_time = ed(rng);
    wait(wait_time, SC_NS);
#else
    // Convert subscription_delay to input rate
    // wait(10*1000*1000, SC_NS);
    // wait(200, SC_NS);
    // wait(10*100*10, SC_NS);
    wait(0.5, SC_NS);
#endif

    std::size_t context         = 1;  // uid_contexts(rng);
    std::size_t isolation_group = uid_isolation_groups(rng);

    Packet p(i, context, -1, packet_gen());

    auto data_out = std::make_tuple(p, context, isolation_group);

    std::shared_ptr<InputStimulus> pkt
          = std::make_shared<InputStimulus>(i, data_out);
    out->put(pkt);
    // cout<<"Packet Generator wrote pktid: "<<i<<endl;
    npulog(profile, cout << "Packet Generator wrote pktid: " << i << endl;)
  }
}
