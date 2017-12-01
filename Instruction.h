/*
 * Instruction.h
 *
 *  Created on: 29 бреб 2017
 *      Author: Eitan Levin
 */

#ifndef INSTRUCTION_H_
#define INSTRUCTION_H_

#define DATA_GARBAGE (-1)
#define STORE_COM 1
#define LOAD_COM 0
#define HIT 1
#define MISS 0

class Instruction {
public:
	int _inst_id; // 0-load, 1-store
	int _addr;	// load / store address
	int _data; // store data / garbage on load
	Instruction(int inst_id, int address, int data = DATA_GARBAGE) {
		this->_inst_id = inst_id;
		this->_addr = address;
		this->_data = data;
	}

	const int ID();//returns ID
	const int Address();//returns Address
	const int Data();//returns data

};



#endif /* INSTRUCTION_H_ */
