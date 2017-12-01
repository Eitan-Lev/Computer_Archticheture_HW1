/*
 * Cache.h
 *
 *  Created on: 29 бреб 2017
 *      Author: Eitan Levin
 */

#ifndef CACHE_H_
#define CACHE_H_

#include <list>
#include <vector>
#include <iostream>
#include "Metadata.h"
#include "Instruction.h"

extern Metadata* MemArray;

#define EVICTION_POLICY_RANDOM 0
#define EVICTION_POLICY_LRU 1
#define INIT_RANDOM 1
#define INIT_FF 255
#define DEFAULT_BLOCK_SIZE 4
#define DEFAULT_CACHE_SIZE 128
#define DEFAULT_ASSOCIATIVITY 1

using namespace std;

class Cache {
private:
	int _cache_size;
	int _block_size;
	int _k_way_associativity;
	bool _eviction_policy;
	bool _init_policy;
	int _sets_number;
//	std::vector<(std::list<Metadata>)> sets;
//	std::vector<std::list<Metadata> SetArray> sets;
	list<Metadata>* SetArrays;
public:
	/*
	 * Constructor.
	 * Send parameters in this order. If no parameters to main, send nothing.
	 */
	Cache(int cache_size = DEFAULT_BLOCK_SIZE, int block_size = DEFAULT_BLOCK_SIZE,
			int associativity = DEFAULT_ASSOCIATIVITY,
			int eviction_policy = EVICTION_POLICY_LRU, int init_policy = INIT_FF) {
		this->_cache_size = cache_size;
		this->_block_size = block_size;
		this->_k_way_associativity = associativity;
		this->_eviction_policy = eviction_policy;
		this->_init_policy = init_policy;
		_sets_number = cache_size/associativity;
		this->SetArrays = new list<Metadata>[_sets_number];
	}
	void StoreInstruction(Instruction inst) {
		int set_number = (inst.address()) % (this->_sets_number);//which list
		list<Metadata>::iterator it;
		Metadata temp;
		temp._data = inst.data();
		temp._address = inst.address();
		MemArray[temp._address] = temp;//Always write to memory because of write through
		//Going over the relevant list and checking if the block exists in it:
		for (it = SetArrays[set_number].begin(); it != SetArrays[set_number].end(); it++) {
			if (it->_address == inst.address()) {
				SetArrays[set_number].erase(it);
				SetArrays[set_number].push_front(temp);
				return;
			}
		}
		//If reached here, this address did not exist in the cache:
		if (SetArrays[set_number].size() >= this->_k_way_associativity) {
			temp = SetArrays[set_number].back();
			MemArray[temp._address] = temp;
			SetArrays[set_number].pop_back();
		}
		SetArrays[set_number].push_front(temp);
	}

	void PrintAllCache() {//TODO only internal usage
		list<Metadata>::iterator it;
		for (int i = 0; i < this->_sets_number; i++) {
			cout << "Set Number: " << i << endl;
			for (it = SetArrays[i].begin(); it != SetArrays[i].end(); it++) {
				cout << "address is: " << it->_address <<
						", data is: " << it->_data << endl;
			}
		}
	}
};

//class CacheMember {
//public:
//	list<Metadata> SetArray;
//	int last_updated;
//};


#endif /* CACHE_H_ */
