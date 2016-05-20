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

/*
 *  Created by UA on 24/04/15.
 */

#ifndef TRIESYSTEMC_SYSCMODULES_H
#define TRIESYSTEMC_SYSCMODULES_H

#endif //TRIESYSTEMC_SYSCMODULES_H


#include "systemc.h"
using namespace sc_core;
using namespace sc_dt;
using namespace std;

#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/simple_target_socket.h"

#include "Trie.h"
#include "PrefixTree.h"
#include "LCTrie.h"
#include "HashTrie.h"
#include "MultibitTrie.h"

#include "configdebug.h"


#include "TrieSqliteInterface.h"

#include "../../CDU.h"
#include "../../TlmVar.h"
#include "../../tlmdefines.h"

using namespace std;

enum { SIZE = 16192*32*32 };
//***************Memory Module
struct Memory: sc_module {
	// TLM-2 socket, defaults to 32-bits wide, base protocol
	tlm_utils::simple_target_socket<Memory> socket;

	SC_CTOR(Memory)
	: socket("socket")
	{
		// Register callback for incoming b_transport interface method call
		socket.register_b_transport(this, &Memory::b_transport);

		// Initialize memory with random data
		for (int i = 0; i < SIZE; i++)
			mem[i] = 0xAA000000 | (rand() % 256);
	}

	// TLM-2 blocking transport method
	virtual void b_transport( tlm::tlm_generic_payload& trans, sc_time& delay );

	std::size_t mem[SIZE];
};
//SC_MODULE (trie_module) {
class trie_module :sc_module {
public:

	tlm_utils::simple_initiator_socket<trie_module> socket_t;


	SC_CTOR (trie_module): socket_t("socket")

	{
		//Prefixtree = new PrefixTree();

		SC_THREAD(insert_into_tree);

	}
	~trie_module(){

		cout<<"Desctructor of Trie Module MemUsage:"<<get_mem_usage()<<endl;
	}
    Trie<void*>* Prefixtree ;
    std::string prefix = "101011";
    std::string nexthop = "10101111";




	//For loadbdfile to hold prefix and hops.
	std::vector<std::string> prefixs ;
	std::vector<std::string> hops;
	std::vector<std::string> testpresfixs;

	void insert_into_tree();
	std::size_t allocate_mem(int size_of_data);
	void allocate(std::size_t data_to_allocate,int addr);
	std::size_t read_mem(int addr);
	void pool_allocator(size_t size_of_pool);
	void loaddbfile();
	void loadtestvectors();
	long unsigned int get_mem_usage();
	void testlarge();
	int addrcounter=0;

};
/*
 * Instance to link pointer for use in allocater in unibitnode allocation.
 * In top module set pointer_to_trie to trie module created, which will allow global access of methods trie module via scoping S::getInstance().pointer_to_tree
 *
 */
class S
{
public:
	static S& getInstance(){
		static S instance; // Guaranteed to be destroyed. // Instantiated on first use.
		//std::cout <<"SINGLETON CLASS"<<std::endl;
		return instance;
	}
	//trie_module* pointer_to_tree;
#ifndef usetlmvar
	CDU* pointer_to_tree;
#endif

#ifdef usetlmvar
	tlmvar* pointer_to_tree;
#endif

private:
	S() {					// Constructor? (the {} brackets) are needed here.

#if debug_singleton_tree_ref
		std::cout << "Singleton object instantiated"<<std::endl;
#endif
	};
	S(S const&)               = delete; // delete copies
	void operator=(S const&)  = delete; //delete instantiation




};
