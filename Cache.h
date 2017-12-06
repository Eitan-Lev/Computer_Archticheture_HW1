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

	/*
	 * For store instruction.
	 * In case of LRU: 	Updates both the cache, and the memory.
	 * 					If the block already exists, move it to the top of the list and updates it.
	 * 					If not, allocate room in the cache for it (on the top).
	 * In case of Random:	Updates both the cache, and the memory.
	 * 						If the block already exists, updates it.
	 * 						If not, allocate room in the cache for it.
	 */
	bool StoreInstruction(Instruction inst);

	/*
	 * For load instruction.
	 * In case of LRU: 	If the block already exists, move it to the top of the list.
	 * 					If not, allocate room in the cache for it (on the top).
	 * In case of Random:	If the block already exists- just return the data.
	 * 						If not, allocate room in the cache for it.
	 */
	LoadResult LoadInstruction(Instruction inst);

	/*
	 * Print the entire cache. For Internal use only!
	 */
	void PrintAllCache();
private:

	/*
	 * Remove a single block, in case the set is full.
	 * If LRU:	remove the last element.
	 * If Random: Remove a random element.
	 */
	void RemoveSingleElementIfListFull(int set_number);

	/*
	 * The function which removes a random block from the set.
	 */
	void RemoveFromMidList(int set_number);
};



#endif /* CACHE_H_ */
