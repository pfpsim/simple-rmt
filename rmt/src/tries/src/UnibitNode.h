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
//  UnibitNode.h
//

#ifndef __UnibitNode__
#define __UnibitNode__

#include "BitString.h"
#include "Trie.h"
#include "systemc.h"
using namespace std;

template <class T>
class UnibitNode {
public:

    // Constructor
    UnibitNode(T iAction, int iActionSize, bool iActionFlag = true, UnibitNode *iOneNodePtr = 0, UnibitNode *iZeroNodePtr = 0);
    UnibitNode();
    UnibitNode(const UnibitNode &iCopyNode);

    // Destructor
    virtual ~UnibitNode();

    // Getters
    UnibitNode* getOneNode() const;
    UnibitNode* getZeroNode() const;
    T getAction() const;
    int getActionSize() const;
    bool getActionFlag() const;

    // Setters
    void setOneNode(UnibitNode* iNewNode);
    void setZeroNode(UnibitNode* iNewNode);
    void setAction(T iNewAction, int iActionSize, bool iActionFlag = true);
    void setActionFlag(bool iFlag);

private:
    T mAction;
    bool mActionFlag;
    int mActionSize;
    UnibitNode *mOneNodePtr;
    UnibitNode *mZeroNodePtr;
};

//
//  UnibitNode.cpp
//

//#include "UnibitNode.h"

/// ==========================
//
//  Default Constructors
//
/// ==========================

template <class T>
UnibitNode<T>::UnibitNode() : mActionFlag(false), mOneNodePtr(0), mZeroNodePtr(0) {}

template <class T>
UnibitNode<T>::UnibitNode(T iAction, int iActionSize, bool iActionFlag, UnibitNode *iOneNodePtr, UnibitNode *iZeroNodePtr) : mAction(iAction), mActionSize(iActionSize), mActionFlag(iActionFlag), mOneNodePtr(iOneNodePtr), mZeroNodePtr(iZeroNodePtr) {
}

/// ==========================
//
//  Copy Constructor
//
/// ==========================

template <class T>
UnibitNode<T>::UnibitNode(const UnibitNode &iCopyNode) {
    mAction = iCopyNode.mAction;
    mActionSize = iCopyNode.mActionSize;
    mActionFlag = iCopyNode.mActionFlag;
    if (0 != iCopyNode.mOneNodePtr) {
        mOneNodePtr = new UnibitNode(*iCopyNode.mOneNodePtr);
    } else {
        mOneNodePtr = 0;
    }
    if (0 != iCopyNode.mZeroNodePtr) {
        mZeroNodePtr = new UnibitNode(*iCopyNode.mZeroNodePtr);
    } else {
        mZeroNodePtr = 0;
    }
}

/// ==========================
//
//  Destructor
//
/// ==========================

template <class T>
UnibitNode<T>::~UnibitNode() {
    if (0 != mOneNodePtr) {
        delete mOneNodePtr;
        mOneNodePtr = 0;
    }

    if (0 != mZeroNodePtr) {
        delete mZeroNodePtr;
        mZeroNodePtr = 0;
    }
}

/// =========================
//
//  Getters
//
/// =========================

template <class T>
UnibitNode<T>* UnibitNode<T>::getOneNode() const {
    // wait(1,SC_NS);
    return mOneNodePtr;
}

template <class T>
UnibitNode<T>* UnibitNode<T>::getZeroNode() const {
    // wait(1,SC_NS);
    return mZeroNodePtr;
}

template <class T>
T UnibitNode<T>::getAction() const {
    // wait(1,SC_NS);
    return mAction;
}

template <class T>
int UnibitNode<T>::getActionSize() const {
    // wait(1,SC_NS);
    return mActionSize;
}

template<class T>
bool UnibitNode<T>::getActionFlag() const {
    // wait(1,SC_NS);
    return mActionFlag;
}

/// =========================
//
//  Setters
//
/// =========================

template <class T>
void UnibitNode<T>::setOneNode(UnibitNode* iNewNode) {
    // wait(2,SC_NS);
    mOneNodePtr = iNewNode;
}

template <class T>
void UnibitNode<T>::setZeroNode(UnibitNode* iNewNode) {
    // wait(2,SC_NS);
    mZeroNodePtr = iNewNode;
}

template <class T>
void UnibitNode<T>::setAction(T iNewAction, int iActionSize, bool iActionFlag) {
    // wait(6,SC_NS);
    mAction = iNewAction;
    mActionSize = iActionSize;
    mActionFlag = iActionFlag;
}

template <class T>
void UnibitNode<T>::setActionFlag(bool iFlag) {
    // wait(2,SC_NS);
    mActionFlag = iFlag;
}

#endif /* defined(__UnibitNode__) */
