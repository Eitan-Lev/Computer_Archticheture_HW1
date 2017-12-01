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

class Instruction {
private:
	int _inst_id; // 0-load, 1-store
	int _addr;	// load / store address
	int _data; // store data / garbage on load
public:
	Instruction(int inst_id, int address, int data = DATA_GARBAGE) {
		this->_inst_id = inst_id;
		this->_addr = address;
		this->_data = data;
	}

	int& ID();//returns ID
	int& address();//returns Address
	int& data();//returns data

};



#endif /* INSTRUCTION_H_ */
