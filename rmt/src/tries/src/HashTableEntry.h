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
//  HashTableEntry.h
//

#ifndef __Trie_Data_Structure__HashTableEntry__
#define __Trie_Data_Structure__HashTableEntry__

#include <functional>
#include "Trie.h"
#include "systemc.h"
template <class T>
class HashTableEntry {
public:
    // Constructor
    HashTableEntry(bool iFlag, unsigned int iKey_Bit, int iKeyLength, T iVal, HashTableEntry<T>* iPtr);
    HashTableEntry();
    HashTableEntry(const HashTableEntry &iCopy);

    // Destructor
    ~HashTableEntry();

    // Getters
    bool getFlag() const;
    unsigned int getKeyBitmap() const;
    T getValue() const;
    HashTableEntry<T>* getPtr() const;
    short getKeyLength() const;
    int getValPtrSize() const;

    // Setters
    void setFlag(bool iFlag);
    void setKeyBitmap(int iKey_Bit);
    void setValue(T iVal, int iSize);
    void setPtr(HashTableEntry<T>* iPtr, int iSize);
    void setKeyLength(short iLength);

private:
    bool mEntryTypeFlag; // 1 - key/value pair, 0 - bitmap/pointer pair
    unsigned int mKey_Bitmap;
    T mValue;
    HashTableEntry<T>* mPtr;
    int mVal_PtrSize;
    short mKeyLength;

};

//
//  HashTableEntry.cpp
//  Trie Data Structure
//
//  Created by Eric Tremblay on 2015-03-25.
//  Copyright (c) 2015 Eric Tremblay. All rights reserved.
//

//#include "HashTableEntry.h"

/// ==========================
//
//  Constructors
//
/// ==========================

template <class T>
HashTableEntry<T>::HashTableEntry(bool iFlag, unsigned int iKey_Bit, int iKeyLength, T iVal, HashTableEntry<T>* iPtr) : mEntryTypeFlag(iFlag), mKey_Bitmap(iKey_Bit), mValue(iVal), mPtr(iPtr), mKeyLength(iKeyLength) {}

template <class T>
HashTableEntry<T>::HashTableEntry() : mEntryTypeFlag(0), mKey_Bitmap(0), mKeyLength(0), mPtr(0) {}

/// ==========================
//
//  Copy Constructor
//
/// ==========================

template <class T>
HashTableEntry<T>::HashTableEntry(const HashTableEntry<T>& iCopy) : mEntryTypeFlag(iCopy.getFlag()), mKey_Bitmap(iCopy.getKeyBitmap()), mKeyLength(iCopy.getKeyLength()), mValue(iCopy.getValue()), mPtr(iCopy.getPtr()) {}

/// ==========================
//
//  Destructor
//
/// ==========================

template <class T>
HashTableEntry<T>::~HashTableEntry() {
    if (mEntryTypeFlag == 0 && mPtr != 0) {
        delete [] mPtr;
        mPtr = 0;
    }
}

/// ==========================
//
//  Getters
//
/// ==========================

template <class T>
bool HashTableEntry<T>::getFlag() const {
    // wait(1,SC_NS);
    return mEntryTypeFlag;
}

template <class T>
unsigned int HashTableEntry<T>::getKeyBitmap() const {
    // wait(1,SC_NS);
    return mKey_Bitmap;
}

template <class T>
T HashTableEntry<T>::getValue() const {
    // wait(1,SC_NS);
    return mValue;
}

template <class T>
HashTableEntry<T>* HashTableEntry<T>::getPtr() const {
    // wait(1,SC_NS);
    return mPtr;
}

template <class T>
short HashTableEntry<T>::getKeyLength() const {
    // wait(1,SC_NS);
    return mKeyLength;
}

template <class T>
int HashTableEntry<T>::getValPtrSize() const {
    // wait(1,SC_NS);
    return mVal_PtrSize;
}

/// ==========================
//
//  Setters
//
/// ==========================

template <class T>
void HashTableEntry<T>::setFlag(bool iFlag) {
    // wait(2,SC_NS);

    mEntryTypeFlag = iFlag;
}

template <class T>
void HashTableEntry<T>::setKeyBitmap(int iKey_Bit) {
    // wait(2,SC_NS);

    mKey_Bitmap = iKey_Bit;
}

template <class T>
void HashTableEntry<T>::setValue(T iVal, int iSize) {
    // wait(2,SC_NS);
    // wait(2,SC_NS);

    mValue = iVal;
    mVal_PtrSize = iSize;
}

template <class T>
void HashTableEntry<T>::setPtr(HashTableEntry<T>* iPtr, int iSize) {
    // wait(2,SC_NS);

    mPtr = iPtr;
    mVal_PtrSize = iSize;
}

template <class T>
void HashTableEntry<T>::setKeyLength(short iLength) {
    // wait(2,SC_NS);

    mKeyLength = iLength;
}

#endif /* defined(__Trie_Data_Structure__HashTableEntry__) */
