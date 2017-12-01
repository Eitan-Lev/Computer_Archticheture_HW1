/*
 * Cache.h
 *
 *  Created on: 29 бреб 2017
 *      Author: Eitan Levin
 */

#ifndef CACHE_H_
#define CACHE_H_

#include <time.h>
#include <stdlib.h>
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

class LoadResult {
public:
	bool _CacheStatus;
	int _LoadData;
	LoadResult(int cache_status, int data) {
		this->_CacheStatus = cache_status;
		this->_LoadData = data;
	}
	const bool CacheStatus() {
		return this->_CacheStatus;
	}
	const int LoadData() {
		return this->_LoadData;
	}
};

class Cache {
private:
	int _cache_size;
	int _block_size;
	unsigned int _k_way_associativity;
	bool _eviction_policy;
	bool _init_policy;
	int _sets_number;
	list<Metadata>* SetArrays;
public:
	/*
	 * Constructor.
	 * Send parameters in this order. If no parameters to main, send nothing.
	 */
	Cache(int cache_size = DEFAULT_BLOCK_SIZE, int block_size = DEFAULT_BLOCK_SIZE,
			unsigned int associativity = DEFAULT_ASSOCIATIVITY,
			int eviction_policy = EVICTION_POLICY_LRU, int init_policy = INIT_FF) {
		this->_cache_size = cache_size;
		this->_block_size = block_size;
		this->_k_way_associativity = associativity;
		this->_eviction_policy = eviction_policy;
		this->_init_policy = init_policy;
		_sets_number = cache_size/(associativity*block_size);
		this->SetArrays = new list<Metadata>[_sets_number];
	}
	bool StoreInstruction(Instruction inst) {
		int set_number = (inst.Address()) % (this->_sets_number);//which list
		list<Metadata>::iterator it;
		Metadata newInst;
		newInst._data = inst.Data();
		newInst._address = inst.Address();
		MemArray[newInst.Address()] = newInst;//Always write to memory because of write through
		//Going over the relevant list and checking if the block exists in it:
		for (it = SetArrays[set_number].begin(); it != SetArrays[set_number].end(); it++) {
			if (it->Address() == inst.Address()) {
				Metadata eitan;
				eitan._address = it->Address();
				eitan._data = it->Data();
				SetArrays[set_number].remove(eitan);//TODO change from eitan
				SetArrays[set_number].push_front(newInst);
				return HIT;
			}
		}
		//If reached here, this address did not exist in the cache:
		if (SetArrays[set_number].size() >= this->_k_way_associativity) {
			if (this->_eviction_policy == EVICTION_POLICY_RANDOM) {
				RemoveFromMidList(set_number);
			} else {
				SetArrays[set_number].pop_back();
			}
//			Metadata temp = SetArrays[set_number].back();
			//TODO check if ok to delete
//			MemArray[temp._address] = temp;
//			MemArray[temp._address]._address = temp._address;
//			MemArray[temp._address]._data = temp._data;
		}
		MemArray[newInst._address] = newInst;
		SetArrays[set_number].push_front(newInst);
		return MISS;
	}

	//TODO move to private
	void RemoveFromMidList(int set_number) {
		list<Metadata>::iterator iter = SetArrays[set_number].begin();
		unsigned int rnd = rand() % (this->_k_way_associativity);//A random value between 0 and sets number - 1
		cout << "rand = " << rnd << endl;
		if (SetArrays[set_number].size() == rnd + 1) {
			iter = SetArrays[set_number].end();
			iter--;
		} else if (rnd != 0) {
			for (unsigned int i = 0; i <= rnd; iter++, i++) {
				if (iter == SetArrays[set_number].end()) {
					cout << "bad" << endl;
				}
			}
		}
		SetArrays[set_number].erase(iter);
	}

	LoadResult LoadInstruction(Instruction inst) {
		int set_number = (inst.Address()) % (this->_sets_number);//which set
		list<Metadata>::iterator it;
		Metadata CurrInst;
		CurrInst._address = inst.Address();
		//Going over the relevant list and checking if the block exists in it:
		for (it = SetArrays[set_number].begin(); it != SetArrays[set_number].end(); it++) {
			//If found, move it to the top of the list for lru
			if (it->_address == inst.Address()) {
				CurrInst._address = it->Address();
				CurrInst._data = it->Data();
				SetArrays[set_number].remove(CurrInst);
				SetArrays[set_number].push_front(CurrInst);
				//TODO mention it is a hit
				LoadResult loadRes(HIT, CurrInst.Data());
				return loadRes;
			}
		}
		//If reached here, this address did not exist in the cache:
		if (SetArrays[set_number].size() >= this->_k_way_associativity) {
			if (this->_eviction_policy == EVICTION_POLICY_RANDOM) {
				RemoveFromMidList(set_number);
			} else {
				SetArrays[set_number].pop_back();
			}
		}
		CurrInst = MemArray[CurrInst.Address()];
		SetArrays[set_number].push_front(CurrInst);
		LoadResult loadRes(MISS, CurrInst.Data());
		return loadRes;
	}

	void PrintAllCache() {//TODO only internal usage
		list<Metadata>::iterator it;
		for (int i = 0; i < this->_sets_number; i++) {
			if (SetArrays[i].empty() == false) {
				cout << "Set Number: " << i << endl;
			}
			for (it = SetArrays[i].begin(); it != SetArrays[i].end(); it++) {
				cout << "address is: " << it->_address <<
						", data is: " << it->_data << endl;
			}
		}
	}
};


#endif /* CACHE_H_ */
