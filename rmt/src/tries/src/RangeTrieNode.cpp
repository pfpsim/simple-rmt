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
//  RangeTrieNode.cpp
//  Trie Data Structure
//
//  Created by Eric Tremblay on 2015-05-29.
//  Copyright (c) 2015 Eric Tremblay. All rights reserved.
//

#include "RangeTrieNode.h"

RangeTrieNode::RangeTrieNode() : mComparisons(0), mAddressAlignment(0), mCommonPrefix(0), mCommonPrefixLength(0), mCommonSuffix(0), mCommonSuffixLength(0), mCompareLength(0), mComparePosition(0), mNextNodes(0), mNumOfComparisons(0), mRegionIDs(0) {}

RangeTrieNode::~RangeTrieNode() {
    if (mComparisons) {
        delete [] mComparisons;
        mComparisons = 0;
    }
    if (mRegionIDs) {
        delete [] mRegionIDs;
        mRegionIDs = 0;
    }
    if (mNextNodes) {
        delete [] mNextNodes;
    }
}


// Getters
unsigned int* RangeTrieNode::getComparisons() const {
    wait(1,SC_NS);

    return mComparisons;
}

unsigned int RangeTrieNode::getNumOfComparisons() const {
    wait(1,SC_NS);

    return mNumOfComparisons;
}

unsigned int RangeTrieNode::getComparePosition() const {
    wait(1,SC_NS);

    return mComparePosition;
}

unsigned int RangeTrieNode::getCompareLength() const {
    wait(1,SC_NS);

    return mCompareLength;
}

int RangeTrieNode::getCommonPrefix() const {
    wait(1,SC_NS);

    return mCommonPrefix;
}

unsigned int RangeTrieNode::getCommonPrefixLength() const {
    wait(1,SC_NS);

    return mCommonPrefixLength;
}

int RangeTrieNode::getCommonSuffix() const {
    wait(1,SC_NS);

    return mCommonSuffix;
}

unsigned int RangeTrieNode::getCommonSuffixLength() const {
    wait(1,SC_NS);

    return mCommonSuffixLength;
}

bool RangeTrieNode::getAddressAlignment() const {
    wait(1,SC_NS);

    return mAddressAlignment;
}

RangeTrieNode* RangeTrieNode::getNextNodes() const {
    wait(1,SC_NS);

    return mNextNodes;
}

int* RangeTrieNode::getRegionIDs() const {
    wait(1,SC_NS);

    return mRegionIDs;
}

// Setters
void RangeTrieNode::setComparisons(unsigned int* iComparisons) {
    wait(2,SC_NS);

    mComparisons = iComparisons;
}

void RangeTrieNode::setNumOfComparisons(unsigned int iNum) {
    wait(2,SC_NS);

    mNumOfComparisons = iNum;
}

void RangeTrieNode::setComparePosition(unsigned int iPos) {
    wait(2,SC_NS);

    mComparePosition = iPos;
}

void RangeTrieNode::setCompareLength(unsigned int iLength) {
    wait(2,SC_NS);

    mCompareLength = iLength;
}

void RangeTrieNode::setCommonPrefix(int iPrefix) {
    wait(2,SC_NS);

    mCommonPrefix = iPrefix;
}

void RangeTrieNode::setCommonPrefixLength(unsigned int iLength) {
    wait(2,SC_NS);

    mCommonPrefixLength = iLength;
}

void RangeTrieNode::setCommonSuffix(int iSuffix) {
    wait(2,SC_NS);

    mCommonSuffix = iSuffix;
}

void RangeTrieNode::setCommonSuffixLength(unsigned int iLength) {
    wait(2,SC_NS);

    mCommonSuffixLength = iLength;
}

void RangeTrieNode::setAddressAlignment(bool iAlign) {
    wait(2,SC_NS);

    mAddressAlignment = iAlign;
}

void RangeTrieNode::setNextNodes(RangeTrieNode *iNodes) {
    wait(2,SC_NS);

    mNextNodes = iNodes;
}

void RangeTrieNode::setRegionIDs(int *iIDs) {
    wait(2,SC_NS);

    mRegionIDs = iIDs;
}
