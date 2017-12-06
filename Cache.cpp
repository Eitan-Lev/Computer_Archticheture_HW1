/*
 * Cahce.cpp
 *
 *  Created on: 29 бреб 2017
 *      Author: Eitan Levin
 */

#include "Cache.h"

bool Cache::StoreInstruction(Instruction inst) {
	int set_number = (inst.Address()) % (this->_sets_number);//which list
	list<Metadata>::iterator it;
	Metadata newInst;
	newInst._data = inst.Data();
	newInst._address = inst.Address();
	MemArray[newInst.Address()] = newInst;//Always write to memory because of write through
	for (it = SetArrays[set_number].begin(); it != SetArrays[set_number].end(); it++) {
		if (it->Address() == inst.Address()) {
			if (this->_eviction_policy == EVICTION_POLICY_LRU) {
				SetArrays[set_number].erase(it);//TODO check if possible
				SetArrays[set_number].push_front(newInst);
			} else {//EVICTION_POLICY_RANDOM
				it->_data = newInst.Data();
			}
			return HIT;
		}
	}
	//If reached here, this address did not exist in the cache:
	RemoveSingleElementIfListFull(set_number);
	MemArray[newInst._address] = newInst;
	SetArrays[set_number].push_front(newInst);
	return MISS;
}

LoadResult Cache::LoadInstruction(Instruction inst) {
	int set_number = (inst.Address()) % (this->_sets_number);//which set
	list<Metadata>::iterator it;
	Metadata newBlock;
	newBlock._address = inst.Address();//In case it is not found in the set.
	newBlock._data = inst.Data();//Keep it!
	//Going over the relevant list and checking if the block exists in it:
	for (it = SetArrays[set_number].begin(); it != SetArrays[set_number].end(); it++) {
		//If found, move it to the top of the list for lru
		if (it->_address == inst.Address()) {
			newBlock._data = it->Data();//Need to do it no matter the policy.
			if (this->_eviction_policy == EVICTION_POLICY_LRU) {
				newBlock._address = it->Address();
				SetArrays[set_number].erase(it);
//					SetArrays[set_number].remove(newBlock);
				SetArrays[set_number].push_front(newBlock);//To keep LRU
				LoadResult loadRes(HIT, newBlock.Data());//Note it is a hit, and attach the data.
			} else {//EVICTION_POLICY_RANDOM
				//Do nothing
			}
//				newBlock._address = it->Address();
//				newBlock._data = it->Data();
//				SetArrays[set_number].erase(it);
////				SetArrays[set_number].remove(newBlock);
//				SetArrays[set_number].push_front(newBlock);
			LoadResult loadRes(HIT, newBlock.Data());//Note it is a hit, and attach the data.
			return loadRes;
		}
	}
	//If reached here, this address did not exist in the cache:
	RemoveSingleElementIfListFull(set_number);
	newBlock = MemArray[newBlock.Address()];//Get updated block from memory
	SetArrays[set_number].push_front(newBlock);//Add to cache since it wasn't already inside
	LoadResult loadRes(MISS, newBlock.Data());//Note it is a miss, and attach the data.
	return loadRes;
}

void Cache::PrintAllCache() {//Only internal usage
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

void Cache::RemoveSingleElementIfListFull(int set_number) {
	if (SetArrays[set_number].size() >= this->_k_way_associativity) {
		if (this->_eviction_policy == EVICTION_POLICY_RANDOM) {
			RemoveFromMidList(set_number);
		} else {//EVICTION_POLICY_LRU
			SetArrays[set_number].pop_back();
		}
	}
}

void Cache::RemoveFromMidList(int set_number) {
	list<Metadata>::iterator iter = SetArrays[set_number].begin();
	unsigned int rnd = rand() % (this->_k_way_associativity);//A random value between 0 and sets number - 1
	if (SetArrays[set_number].size() == rnd + 1) {//If last element, for loop does not work
		iter = SetArrays[set_number].end();
		iter--;
	} else if (rnd != 0) {//If not first element, everything works.
		for (unsigned int i = 0; i <= rnd; iter++, i++) {
			if (iter == SetArrays[set_number].end()) {
				cout << "bad" << endl;//TODO remove last
			}
		}
	}
	SetArrays[set_number].erase(iter);
}
