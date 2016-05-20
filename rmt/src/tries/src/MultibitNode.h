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
//  MultibitNode.h
//  Trie Data Structure
//
//  Created by Eric Tremblay on 2015-03-12.
//  Copyright (c) 2015 Eric Tremblay. All rights reserved.
//

#ifndef __Trie_Data_Structure__MultibitNode__
#define __Trie_Data_Structure__MultibitNode__

#include "BitString.h"
#include "systemc.h"
template <class T>
class MultibitNode {
public:
    // Constructor
    MultibitNode();

    // Destructor
    ~MultibitNode();

    // Setters
    void setStride(unsigned int iStride);
    void setNextNodePtr(MultibitNode<T> *iNextNode);
    void setAction(T iAction, int iActionSize, bool iActionFlag = true);
    void setActionFlag(bool iFlag);
    void setLength(unsigned int iLength);

    // Getters
    unsigned int getStride() const;
    MultibitNode<T>* getNextNodePtr() const;
    T getAction() const;
    int getActionSize() const;
    bool getActionFlag() const;
    unsigned int getLength() const;

private:
    unsigned int mStride;           // Length of prefixes in its next level
    MultibitNode *mNextNodePtr;     // Pointer to the next level
    T mAction;                      // Action information
    int mActionSize;                // Action Size for TLM memory
    unsigned int mLength;           // Length of the prefix whose next hop was assigned to this node
    bool mActionFlag;               // Boolean indicating if the Action is valid

};

//
//  MultibitNode.cpp
//  Trie Data Structure
//
//  Created by Eric Tremblay on 2015-03-12.
//  Copyright (c) 2015 Eric Tremblay. All rights reserved.
//

//#include "MultibitNode.h"

/// ============================
//
//  Constructor
//
/// ============================

template <class T>
MultibitNode<T>::MultibitNode() : mStride(0), mNextNodePtr(0), mLength(0), mActionFlag(false) {}

/// ============================
//
//  Destructor
//
/// ============================

template <class T>
MultibitNode<T>::~MultibitNode() {}

/// ============================
//
//  Setters
//
/// ============================

template <class T>
void MultibitNode<T>::setStride(unsigned int iStride) {
    wait(2,SC_NS);

    mStride = iStride;
}

template <class T>
void MultibitNode<T>::setNextNodePtr(MultibitNode *iNextNode) {
    wait(2,SC_NS);

    mNextNodePtr = iNextNode;
}

template <class T>
void MultibitNode<T>::setAction(T iAction, int iActionSize, bool iActionFlag) {
    wait(2,SC_NS);
    wait(2,SC_NS);
    wait(2,SC_NS);

    mAction = iAction;
    mActionSize = iActionSize;
    mActionFlag = iActionFlag;
}

template <class T>
void MultibitNode<T>::setActionFlag(bool iFlag) {
    wait(2,SC_NS);

    mActionFlag = iFlag;
}

template <class T>
void MultibitNode<T>::setLength(unsigned int iLength) {
    wait(2,SC_NS);

    mLength = iLength;
}

/// ============================
//
//  Getters
//
/// ============================

template <class T>
unsigned int MultibitNode<T>::getStride() const {
    wait(1,SC_NS);

    return mStride;
}

template <class T>
MultibitNode<T>* MultibitNode<T>::getNextNodePtr() const {
    wait(1,SC_NS);

    return mNextNodePtr;
}

template <class T>
T MultibitNode<T>::getAction() const {
    wait(1,SC_NS);

    return mAction;
}

template <class T>
int MultibitNode<T>::getActionSize() const {
    wait(1,SC_NS);

    return mActionSize;
}
template <class T>
bool MultibitNode<T>::getActionFlag() const {
    wait(1,SC_NS);

    return mActionFlag;
}

template <class T>
unsigned int MultibitNode<T>::getLength() const {
    wait(1,SC_NS);

    return mLength;
}

#endif /* defined(__Trie_Data_Structure__MNode__) */
