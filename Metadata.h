/*
 * Metadata.h
 *
 *  Created on: 29 бреб 2017
 *      Author: Eitan Levin
 */

#ifndef METADATA_H_
#define METADATA_H_

#define DEFAULT_MEM_SIZE 1024

class Metadata {
public:
	int _data;
	int _address;
	Metadata() {
		this->_data = DEFAULT_MEM_SIZE;
		this->_address = (-1);//A random value, no meaning
	}

	bool operator==(const Metadata& block) {
		return (this->_address == block._address);
	}
	const int Data() {
		return this->_data;
	}
	const int Address() {
		return this->_address;
	}
};



#endif /* METADATA_H_ */
