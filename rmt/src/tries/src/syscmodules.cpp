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
        Created by UA on 24/04/15.
*/
#include <math.h>
#include<vector>
#include "syscmodules.h"
#include "freelist.h"
#include "pool_allocator_defines.h"

//Memory Transport Callback
void Memory:: b_transport( tlm::tlm_generic_payload& trans, sc_time& delay )
{
    tlm::tlm_command cmd = trans.get_command();
    sc_dt::uint64    adr = trans.get_address() ;
    unsigned char*   ptr = trans.get_data_ptr();
    unsigned int     len = trans.get_data_length();
    unsigned char*   byt = trans.get_byte_enable_ptr();
    unsigned int     wid = trans.get_streaming_width();

    // check address range
    if (adr >= sc_dt::uint64(SIZE) || byt != 0 || len > 4 || wid < len) {
    	cout<<"Addr"<<adr<<endl
    		<<"Size"<<sc_dt::uint64(SIZE)<<endl
    		<<"byt"	<<byt<<endl
			<<"len"	<<len<<endl
			<<"wid"	<<wid<<endl;
        SC_REPORT_ERROR("TLM-2", "Target does not support given generic payload transaction");
    }
    // Read or Write depending upon command.
    if ( cmd == tlm::TLM_READ_COMMAND ) {
        memcpy(ptr, &mem[adr], len);
    }
    else if ( cmd == tlm::TLM_WRITE_COMMAND ) {
        memcpy(&mem[adr], ptr, len);
    }
    trans.set_response_status( tlm::TLM_OK_RESPONSE );     //response status to indicate successful completion
}

//Load test data from file
void trie_module::loaddbfile(){
#ifndef TLM_TRIES_EXCLUDE_TESTS
	std::cout << "..Parsing DB File.."<<std::endl;

    ifstream iFile("/home/samar/workspace/trieallocator_all/build/dblog.csv");

    std::string delimiter = ",";
    string line;

    while(getline(iFile, line)) { /* While there is still a line. */

    	line.erase(std::remove(line.begin(), line.end(), ' '), line.end()); 	// Remove Whitespace in extracted line.
    	//cout<<"Extracted Line: "<<line<<endl;
        size_t pos = 0;
        std::string token; 														//store extracted substring
        while ((pos = line.find(delimiter)) != std::string::npos) { 			//get token position based on delimiter
            token = line.substr(0, pos); 										// extract sub-string from start of string to position of delimiter
            //std::cout << token << std::endl;
            prefixs.push_back(std::move(token)); 								//Save Prefix in vector
            line.erase(0, pos + delimiter.length()); 							//Erase Prefix+Delimiter from main string
        }
        //std::cout << line << std::endl;
        line.erase(std::remove(line.begin(), line.end(), '\r'), line.end()); 	//strip new line from end.
        hops.push_back(std::move(line)); 										// Save Next Hop

    }
    iFile.close();
    std::cout << "..Loaded Test Data.."<<std::endl<<std::endl;

#endif
}

void trie_module::loadtestvectors(){
#ifndef TLM_TRIES_EXCLUDE_TESTS

	cout<<".:Loading Test Prefix:."<<endl;

	ifstream testvectors("/home/samar/workspace/trieallocator_all/build/testvectors.log");

	if(!testvectors){
		cout<<"File Not found"<<endl;
	}
	string line;

	while(getline(testvectors,line)){
		line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
		testpresfixs.push_back(std::move(line));
	}
	testvectors.close();

#endif
}

void trie_module::testlarge(){
#ifndef TLM_TRIES_EXCLUDE_TESTS


		cout<<endl<<"-Testing Prefixs from db -- Started"<<endl;

		cout<<endl<<"-Started @ "<<sc_time_stamp()<<endl;
		loaddbfile(); //Load test data
		std::cout << "--Inserting All Prefixes-"<<std::endl;
		int count =0 ;
		for (std::vector<std::string>::iterator it = prefixs.begin(), it2=hops.begin() ; it != prefixs.end(); ++it,++it2){
			std::string prefix = *it ;
			std::string nexthop = *it2;
		   // std::cout<<count<<std::endl;
		   //std::cout <<"Prefix: "<<prefix<<" Next Hop: "<<nexthop<<endl;
		 //  Prefixtree->insert(prefix,nexthop);
		   count++;

		   //std::cout<<count<<std::endl;
		}
		std::cout<<"--Prefixes Inserted: "<<count<<endl;

		cout<<endl<<"-Search Start @ "<<sc_time_stamp()<<endl;

		std::cout << "--Searching for All Prefixes"<<std::endl;
		count =0 ;
		for (std::vector<std::string>::iterator it = prefixs.begin(), it2=hops.begin() ; it != prefixs.end(); ++it,++it2){
			   std::string prefix = *it ;
			   std::string nexthop = *it2;
			   //std::cout<<count<<std::endl;
			   std::string result ;
		//	   result= Prefixtree->exactPrefixMatch(prefix);
			  if(!(nexthop==result)){
				  std::cout<<"--Prefix Not Matched"				<<std::endl
						   <<"  |-- Hop Val:"	<<nexthop		<<std::endl
						   <<"  |-- Ret Val:"	<<result		<<std::endl
						   <<"  |-- Count:"		<<count			<<std::endl
						   <<"	|-- Hop size:"	<<nexthop.size()<<std::endl
						   <<"	|-- Ret size:"	<<result.size()	<<std::endl;

			  }
			   count++;
		}
		std::cout<<"--Prefixes Matched: "<<count<<endl;

		cout<<endl<<"-Remove start @ "<<sc_time_stamp()<<endl;

		std::cout << "--Deleting All Prefixes"<<std::endl;
		count =0 ;
		for (std::vector<std::string>::iterator it = prefixs.begin(), it2=hops.begin() ; it != prefixs.end(); ++it,++it2){
		   std::string prefix = *it ;
		   std::string nexthop = *it2;
	//	   Prefixtree->remove(prefix);
		   count++;
		}
		std::cout<<"--Prefixes Deleted: "<<count<<endl;

		cout<<endl<<"-End @ "<<sc_time_stamp()<<endl;

		std::cout<<"Trace Files Written"<<endl;
		std::cout<<"Large Prefixes Test Complete"<<endl;

#endif
}

void trie_module::insert_into_tree() {
#ifndef TLM_TRIES_EXCLUDE_TESTS

	const char* FILEPATH = "/home/samar/workspace/trieallocator_all/src/IPAddressDatabaseDemo.db"; // Database file path

	const char* FILEPATH2 = "/home/samar/workspace/trieallocator_all/src/IPAddressDatabaseDemo2.db"; // Second database file path

	 //Trie *myTrie = 0;
	TrieSqliteInterface *trieSqliteInterface = new TrieSqliteInterface(FILEPATH);
	RoutingTableEntry *prefixes = trieSqliteInterface->getArrayOfAllPrefixes();
	unsigned int numOfPrefixes = trieSqliteInterface->getNumOfPrefixes();


	loadtestvectors();

	long unsigned int var1 = 0;
	cout<<"SIZE: "<<sizeof(var1)<<endl;
	cout<<".:Loaded From db - Sucess:."<<endl;

	cout <<"Auto Test Tries:"	<<endl
		 <<"1.Prefix"			<<endl
		 <<"2.LC"				<<endl
		 <<"3.HashTrie"			<<endl
		 <<"4.MultibitTree"		<<endl;
	int treeselection =0;
	cin>>treeselection;

	//time logging variables
	sc_time start;
	sc_time end;
	sc_time diff;

	 string inputPrefix="";



	 switch(treeselection){
	 	 case 1: // brackets for case is for deterministic scope of variable. C++11/14 Conformancy
	 	 {
	 		cout<<"Prefix Tree"<<endl
			 	<<"-----------------------------------------------------------"<<endl;

	 		cout << "\nBuilding Trie Data Structures..." << endl;
	 		start = sc_time_stamp();
	 		Trie *myPrefixTree = new PrefixTree(prefixes, numOfPrefixes);
	 		end = sc_time_stamp();
	 		diff = end-start;
	 		cout<< "Time it took to build: "<<diff<<endl;
	 		get_mem_usage();
	 		cout<<endl
	 			<<"-----------------------------------------------------------"<<endl<<endl;
	 		std::string result ;

	 		ofstream logfile;
			logfile.open ("treelogs-prefix.csv");
			for (std::vector<std::string>::iterator it = testpresfixs.begin(); it != testpresfixs.end(); ++it){
	 			std::string prefix = *it ;
	 			start = sc_time_stamp();
				result = myPrefixTree->longestPrefixMatch(prefix);
				end = sc_time_stamp();
		 		diff = end-start;
		 		logfile <<prefix<<","<<result<<","<<diff<<"\n";
	 		}
			logfile.close();

			cout <<"Deleting Tree"<<endl;
	 		delete myPrefixTree;

	 	 }
	 		break;

	 	 case 2:
	 	 {
	 		 cout<<"LC Tree"<<endl
			 	<<"-----------------------------------------------------------"<<endl;
	 		cout << "\nBuilding Trie Data Structures..." << endl;

	 		start = sc_time_stamp();
	 		Trie *myLCTrie = new LCTrie(prefixes, numOfPrefixes);
	 		end = sc_time_stamp();
	 		cout<<"Time it took to build:"<<end-start<<endl;
	 		get_mem_usage();

	 		cout<<endl<<"@ "<<sc_time_stamp()<<endl;
	 		cout<<endl
	 			<<"-----------------------------------------------------------"<<endl<<endl;
	 		std::string result ;
	 		ofstream logfile;
			logfile.open ("treelogs-LC.csv");
			for (std::vector<std::string>::iterator it = testpresfixs.begin(); it != testpresfixs.end(); ++it){
				std::string prefix = *it ;
				start = sc_time_stamp();
				result = myLCTrie->longestPrefixMatch(prefix);
				end = sc_time_stamp();
				diff = end-start;
				logfile <<prefix<<","<<result<<","<<diff<<"\n";
			}
			logfile.close();

			cout <<"Deleting Tree"<<endl;
	 		delete myLCTrie;
	 	 }
	 	    break;

	 	 case 3:
	 	 {
	 		cout<<"Hash Tree"<<endl
			 	<<"-----------------------------------------------------------"<<endl;
	 		cout<< "\nBuilding Trie Data Structures..." << endl;

	 		start = sc_time_stamp();
	 		Trie *myHashTrie = new HashTrie(prefixes, numOfPrefixes);
	 		end = sc_time_stamp();
	 		cout<<"Time to build: "<<end-start<<endl;
	 		get_mem_usage();
	 		std::string result ;
	 		ofstream logfile;
			logfile.open ("treelogs-Hash.csv");
			for (std::vector<std::string>::iterator it = testpresfixs.begin(); it != testpresfixs.end(); ++it){
				std::string prefix = *it ;
				start = sc_time_stamp();
				result = myHashTrie->longestPrefixMatch(prefix);
				end = sc_time_stamp();
				diff = end-start;
				logfile <<prefix<<","<<result<<","<<diff<<"\n";
			}
			logfile.close();


			cout <<"Deleting Tree"<<endl;
	 		delete myHashTrie;
	 	 }
	 		break;

	 	 case 4:
	 	 {
	 		 cout<<"Multibit Tree"<<endl
			 	<<"-----------------------------------------------------------"<<endl;
	 		cout << "\nBuilding Trie Data Structures..." << endl;

	 		start = sc_time_stamp();
	 		Trie *myMultiBitTrie = new MultibitTrie(prefixes, numOfPrefixes);
	 		end = sc_time_stamp();
	 		cout<<"Time for Build: "<<end-start<<endl;

	 		get_mem_usage();

	 		 cout<<endl
	 			<<"-----------------------------------------------------------"<<endl;
		 	std::string result ;
	 		ofstream logfile;
			logfile.open ("treelogs-multibittrie.csv");
			for (std::vector<std::string>::iterator it = testpresfixs.begin(); it != testpresfixs.end(); ++it){
				std::string prefix = *it ;
				start = sc_time_stamp();
				result = myMultiBitTrie->longestPrefixMatch(prefix) ;
				end = sc_time_stamp();
				diff = end-start;
				logfile <<prefix<<","<<result<<","<<diff<<"\n";
			}
			logfile.close();


		    cout <<"Deleting Tree"<<endl;
	 		delete myMultiBitTrie;
	 	 }
	 		break;

	 	 case 0:
	 	 {
	 		cout<<"No selection Made"<<endl;
	 	 }
	 		break;
	 	 default:
	 	 {
	 		cout<<"Invalid Selection"<<endl;
	 	 }
	 		break;

	 }
	 cout<<endl<<"@ "<<sc_time_stamp()<<endl;
	 cout<<"----------------------------------------------"<<endl
		 <<"Test End."<<endl;


#endif
}


void trie_module::allocate(std::size_t data_to_allocate, int addr){
//void allocate(int data_to_allocate, int addr){
    tlm::tlm_generic_payload* trans = new tlm::tlm_generic_payload;
    sc_time delay = sc_time(10, SC_NS); //*****************************TODO: MOVE TO MACRO ********************************

    tlm::tlm_command cmd = static_cast<tlm::tlm_command>(1);//0 for read ***** 1 for write
    trans->set_command( cmd );
    trans->set_address( addr );
    trans->set_data_ptr( reinterpret_cast<unsigned char*>(&data_to_allocate) );
    trans->set_data_length( 4 );
    trans->set_streaming_width( 4 ); // = data_length to indicate no streaming ?
    trans->set_byte_enable_ptr( 0 ); // 0 indicates unused
    trans->set_dmi_allowed( false ); // Mandatory initial value
    trans->set_response_status( tlm::TLM_INCOMPLETE_RESPONSE ); // Mandatory initial value

    socket_t->b_transport( *trans, delay );  // Blocking transport call

    if ( trans->is_response_error() ){ // check response status and dela
        SC_REPORT_ERROR("TLM-2", "Response error from b_transport");
    }

	#if debug_tlm_mem_transaction
    cout 	<< "      transaction = { " << (cmd ? 'W' : 'R') << ", " << hex << addr
    		<< " } , data = " << dec << data_to_allocate << " at time " << sc_time_stamp()
			<< " delay = " << delay << endl;
    // delay annotated onto the transport call
	#endif
    wait(delay);
}

std::size_t trie_module::allocate_mem(int size_of_data){

	if(addrcounter<SIZE){ //*** TODO: INSERT CHECK OF SIZE LEFT ALSO

		double alloc_req = (float)size_of_data/(float)32; //get size of data to allocate
	    int count=0;

	    int ret_addr = addrcounter; // current free head of memory block

	    for(int i=addrcounter; i<(ceil(alloc_req)+addrcounter);i++){
	    	//cout<<endl<<hex<<i<<"***"<<endl;
	    	allocate((int)0,i);
	    	count++;
	    }
	    addrcounter=addrcounter+count;
	    //cout<<"Val of addrcounter"<<addrcounter<<endl;
	    return ret_addr;
	}
}


void trie_module::pool_allocator(size_t size_pool){

	size_t memory[size_pool];

	Freelist freelist(memory, memory+size_pool, slot_size, boundry_alignment, offset);


	//Kickstart with two slots

	// allocates a slot of 32 bytes, aligned to an 4-byte boundary
	void* object0 = freelist.Obtain();

	// allocates another slot of 32 bytes, aligned to an 4-byte boundary
	void* object1 = freelist.Obtain();

	// obtained slots can be returned in any order
	freelist.Return(object1);
	freelist.Return(object0);


}


std::size_t trie_module::read_mem(int addr){

	std::size_t read_val=0 ;
    tlm::tlm_generic_payload* trans = new tlm::tlm_generic_payload;
    sc_time delay = sc_time(10, SC_NS);//***************************TODO: MOVE TO MACRO ********************************

    tlm::tlm_command cmd = static_cast<tlm::tlm_command>(0);//0 for read ***** 1 for write
    trans->set_command( cmd );
    trans->set_address( addr );
    trans->set_data_ptr( reinterpret_cast<unsigned char*>(&read_val) );
    trans->set_data_length( 4 );
    trans->set_streaming_width( 4 ); // = data_length to indicate no streaming
    trans->set_byte_enable_ptr( 0 ); // 0 indicates unused
    trans->set_dmi_allowed( false ); // Mandatory initial value
    trans->set_response_status( tlm::TLM_INCOMPLETE_RESPONSE ); // Mandatory initial value

    socket_t->b_transport( *trans, delay );  // Blocking transport call
    if ( trans->is_response_error() ) {
        SC_REPORT_ERROR("TLM-2", "Response error from b_transport");
    }
	#if debug_tlm_mem_transaction
    cout 	<< "      transaction = { " << (cmd ? 'W' : 'R') << ", " << hex << addr
    		<< " } , data = " << dec << read_val << " at time " << sc_time_stamp()<<endl;
	#endif
    wait(delay);
    return read_val;
}

long unsigned int trie_module::get_mem_usage(){

	float memusage = ((float)addrcounter)/1024;
	cout<<"Current Mem usage is "<<memusage<<" kb";
	return addrcounter;
}
