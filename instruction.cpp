/*
 * instruction.cpp
 *
 *  Created on: 29 бреб 2017
 *      Author: Eitan Levin
 */
#include "Instruction.h"

int& Instruction::ID () {
	return this->_inst_id;
}

int& Instruction::address() {
	return this->_addr;
}

int& Instruction::data() {
	return this->_data;
}

