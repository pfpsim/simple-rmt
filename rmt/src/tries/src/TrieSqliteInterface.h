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

//
//  TrieSqliteInterface.h
//  Trie Data Structure
//
//  Created by Eric Tremblay on 2015-01-28.
//  Copyright (c) 2015 Eric Tremblay. All rights reserved.
//

#ifndef __Trie_Data_Structure__TrieSqliteInterface__
#define __Trie_Data_Structure__TrieSqliteInterface__

#include <iostream>
#include <sqlite3.h>
#include <vector>
#include "RoutingTableEntry.h"

using namespace std;

class TrieSqliteInterface {
public:
    // Constructor
    TrieSqliteInterface(const char* iFilePath);

    // Destructor
    virtual ~TrieSqliteInterface();

    // generate an array of all prefixes
    virtual void generateArrayOfAllPrefixes();

    // Getters
    int* getPrefixCounter();
    sqlite3* getDb() const;
    RoutingTableEntry<string>* getArrayOfAllPrefixes() const;
    int getNumOfPrefixes() const;

    void printAllPrefixes() const;

private:
    sqlite3 *mDb;
    string mFilePath;
    int mPrefixCounter;
    RoutingTableEntry<string>* mArrayOfAllPrefixes;
    int mNumOfPrefixes;

    // Callbacks for sqlite queries
    static int sqliteLongestPrefixHelperCallback(void *iData, int iArgc, char **iArgv, char **iColName);
    static int sqliteArrayOfPrefixesCallback(void *iData, int iArgc, char **iArgv, char **iColName);

};

#endif /* defined(__Trie_Data_Structure__TrieSqliteInterface__) */
