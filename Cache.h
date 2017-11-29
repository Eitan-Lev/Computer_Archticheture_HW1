/*
 * Cache.h
 *
 *  Created on: 29 бреб 2017
 *      Author: Eitan Levin
 */

#ifndef CACHE_H_
#define CACHE_H_

#define EVICTION_POLICY_RANDOM 0
#define EVICTION_POLICY_LRU 1
#define INIT_RANDOM 1
#define INIT_FF 0
#define DEFAULT_BLOCK_SIZE 4
#define DEFAULT_CACHE_SIZE 128
#define DEFAULT_ASSOCIATIVITY 1

class Cache {
private:
	int _cache_size;
	int _block_size;
	int _k_way_associativity;
	bool _eviction_policy;
	bool _init_policy;
public:
	/*
	 * Constructor.
	 * Send parameters in this order. If no paramaters to main, send nothing.
	 */
	Cache(int cache_size = DEFAULT_BLOCK_SIZE, int block_size = DEFAULT_BLOCK_SIZE,
			int associativity = DEFAULT_ASSOCIATIVITY,
			int eviction_policy = EVICTION_POLICY_LRU, int init_policy = INIT_FF) {
		this->_cache_size = cache_size;
		this->_block_size = block_size;
		this->_k_way_associativity = associativity;
		this->_eviction_policy = eviction_policy;
		this->_init_policy = init_policy;
	}
};



#endif /* CACHE_H_ */
