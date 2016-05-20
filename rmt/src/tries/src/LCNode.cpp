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
//  LCNode.cpp
//  Trie Data Structure
//
//  Created by Eric Tremblay on 2015-02-08.
//  Copyright (c) 2015 Eric Tremblay. All rights reserved.
//

#include "LCNode.h"
#include "systemc.h"

/// ==================================
//
//  Constructor
//
/// ==================================

LCNode::LCNode(int iBranch, int iSkip) : mBranchFactor(iBranch), mSkip(iSkip), mLeftNodePos(0) {}

/// ==================================
//
//  Destructor
//
/// ==================================

LCNode::~LCNode() {}

/// ==================================
//
//  Getters
//
/// ==================================

int LCNode::getBranchFactor() const {
    wait(1,SC_NS);
	return mBranchFactor;
}

int LCNode::getSkip() const {
    wait(1,SC_NS);
    return mSkip;
}

int LCNode::getLeftNodePos() const {
    wait(1,SC_NS);
    return mLeftNodePos;
}

/// ==================================
//
//  Setters
//
/// ==================================

void LCNode::setBranchFactor(int iBranch) {
	wait(2,SC_NS);
    mBranchFactor = iBranch;
}

void LCNode::setSkip(int iSkip) {
	wait(2,SC_NS);
    mSkip = iSkip;
}

void LCNode::setLeftNodePos(int iPos) {
	wait(2,SC_NS);
    mLeftNodePos = iPos;
}
