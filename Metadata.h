/*
 * Metadata.h
 *
 *  Created on: 29 бреб 2017
 *      Author: Eitan Levin
 */

#ifndef METADATA_H_
#define METADATA_H_

#define DEFAULT_MEM_SIZE 1024

//TODO if Metadata only contains data, remove it
class Metadata {
public:
	int _data;
	int _address;
	Metadata() {
		this->_data = DEFAULT_MEM_SIZE;
		this->_address = (-1);//FIXME change value
	}
//	Metadata(const Metadata& block) {
//		_data = block.Data();
//		_address = block.Address();
//	}
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
