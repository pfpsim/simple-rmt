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
//  TrieSqliteInterface.cpp
//  Trie Data Structure
//
//  Created by Eric Tremblay on 2015-01-28.
//  Copyright (c) 2015 Eric Tremblay. All rights reserved.
//

#include "TrieSqliteInterface.h"

/// ==============================
//
//  Constructor
//
/// ==============================

TrieSqliteInterface::TrieSqliteInterface(const char* iFilePath) : mFilePath(iFilePath), mPrefixCounter(0), mArrayOfAllPrefixes(0), mNumOfPrefixes(0) {
    int wErrorCode = sqlite3_open(iFilePath, &mDb);
    if (wErrorCode) {
        cout << "Can't open database. ";
        throw sqlite3_errmsg(mDb);
    } else {
        //cout << "The database was opened successfully." << endl;
    }
    generateArrayOfAllPrefixes();
}

/// ==============================
//
//  Destructor
//
/// ==============================

TrieSqliteInterface::~TrieSqliteInterface() {
    if (mArrayOfAllPrefixes) {
        delete [] mArrayOfAllPrefixes;
        mArrayOfAllPrefixes = 0;
    }
    sqlite3_close(mDb);
};

/// ==================================
//
//  Getters
//
/// ==================================

int* TrieSqliteInterface::getPrefixCounter() {
    return &mPrefixCounter;
}

sqlite3* TrieSqliteInterface::getDb() const {
    return mDb;
}

RoutingTableEntry<string>* TrieSqliteInterface::getArrayOfAllPrefixes() const {
    return mArrayOfAllPrefixes;
}

int TrieSqliteInterface::getNumOfPrefixes() const {
    return mNumOfPrefixes;
}

/// ==================================================
//
//  Generates an array of Routing Table Entries
//
/// ==================================================

void TrieSqliteInterface::generateArrayOfAllPrefixes() {
    vector<RoutingTableEntry<string>> *wTable = new vector<RoutingTableEntry<string>>();
    char sql_stmt[] = "SELECT * FROM ADDRESSES";
    char *errmsg;
    //cout << "Creating array of database entries...";
    int wErrorCode = sqlite3_exec(mDb, sql_stmt, sqliteArrayOfPrefixesCallback, wTable, &errmsg);
    if (wErrorCode != SQLITE_OK) {
        cout << "An error has occured when creating array of database entries. ";
        throw errmsg;
    } else {
        //cout << "Array of database entries has been created successfully." << endl;
    }
    mArrayOfAllPrefixes = new RoutingTableEntry<string>[wTable->size()]();
    for (int i = 0; i < wTable->size(); i++) {
        mArrayOfAllPrefixes[i].setData((*wTable)[i].getData());
        mArrayOfAllPrefixes[i].setLength((*wTable)[i].getLength());
        mArrayOfAllPrefixes[i].setAction((*wTable)[i].getAction());
        mArrayOfAllPrefixes[i].setActionSize((*wTable)[i].getActionSize());
    }
    mNumOfPrefixes = (int)wTable->size();
    delete wTable;
    wTable = NULL;
}

/// ==================================
//
//  Print all Prefixes
//
/// ==================================

void TrieSqliteInterface::printAllPrefixes() const {
    for (int i = 0; i < mNumOfPrefixes; i++) {
        cout << i << " : " << mArrayOfAllPrefixes[i].getData() << ", " << mArrayOfAllPrefixes[i].getLength() << endl;
    }
}

/// ==================================
//
//  Callbacks for sqlite queries
//
/// ==================================

int TrieSqliteInterface::sqliteLongestPrefixHelperCallback(void *iData, int iArgc, char **iArgv, char **iColName) {
    string *wActualNextHop = (string *)iData;
    string wDatabaseNextHop = iArgv[1];
    if ((*wActualNextHop) == wDatabaseNextHop) {
        return 0;
    }
    return 1;
}

int TrieSqliteInterface::sqliteArrayOfPrefixesCallback(void *iData, int iArgc, char **iArgv, char **iColName) {
    vector<RoutingTableEntry<string>> *wTable = (vector<RoutingTableEntry<string>>*) iData;
    string wPrefix = (string) iArgv[0];
    string wNextHop = (string) iArgv[1];
    int wPrefixSize = (int)wPrefix.size();
    RoutingTableEntry<string> wNewEntry(wPrefix, wPrefixSize, wNextHop, (int)wNextHop.size());
    wTable->push_back(wNewEntry);
    return 0;
}






