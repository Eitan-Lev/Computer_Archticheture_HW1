/*
 * cachesim.cpp
 *
 *  Created on: 29 бреб 2017
 *      Author: Eitan Levin
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "Instruction.h"
#include "Metadata.h"
#include "Cache.h"

char line[80];
char* inst_filename;
int mem_size  = DEFAULT_MEM_SIZE;
int cache_size = DEFAULT_CACHE_SIZE;
int block_size = DEFAULT_BLOCK_SIZE;
int associtivity = DEFAULT_ASSOCIATIVITY;
int eviction_policy = EVICTION_POLICY_LRU;
int init_policy = INIT_FF;
FILE *fr;

Metadata* MemArray;



// Converts an Hex string into a decimal integer
int HexToDecimal (char* HexNumber);

// Get a line from the program and parse its values into an "instruction" struct.
int split_inst(char* line, struct instruction* inst);

// Returns value which is located on specific bits of "data": data[high_bound:low_bound]
int getbits(int data, int high_bound, int low_bound);


int HexToDecimal (char* HexNumber)
{
	char* pStopString;
	return (int) strtol (HexNumber, &pStopString, 16);
}


Instruction split_inst(char* line)
{
	Instruction inst(DATA_GARBAGE, DATA_GARBAGE, DATA_GARBAGE);
	char* tokens;
	int token_id=0;
	tokens = strtok (line," \n");
	while (tokens != NULL)
	{
		switch (token_id) {
			case 0: inst.ID()=(!strcmp(tokens,"store")); // 1 if store, else 0
					break;
			case 1: inst.address()=HexToDecimal(tokens); // store addr
					break;
			case 2: inst.data()=HexToDecimal(tokens); // store data
					break;
		}
		token_id++;
		tokens = strtok (NULL, " \n");
	}
	return inst;
}


int getbits(int data, int high_bound, int low_bound)
{
	int tempval=data;
	if (high_bound < 31) tempval = data % (1<<(high_bound+1));
	tempval = tempval / (1<<low_bound);
	return tempval;
}

void Initialize_Memory(int size, int init_value) {
	MemArray = new Metadata[size];
	for (int i = 0; i < size; i++) {
		MemArray[i]._data = init_value;
	}
//	return MemArray;
}

int main(int argc, char* argv[])
{
	int i;
	for (i = 1; i < argc; i++) { 	/* We will iterate over argv[] to get the parameters stored inside.
										* Note that we're starting on 1 because we don't need to know the
										* path of the program, which is stored in argv[0] */
		if (i + 1 != argc) {// Check that we haven't finished parsing already
			if (!strcmp(argv[i],"-f")) {
				inst_filename = argv[i + 1];
			} else if (!strcmp(argv[i],"-m")) {
				mem_size = atoi (argv[i + 1]);
			} else if (!strcmp(argv[i],"-c")) {
				cache_size = atoi (argv[i + 1]);
			} else if (!strcmp(argv[i],"-b")) {
				block_size = atoi (argv[i + 1]);
			} else if (!strcmp(argv[i],"-a")) {
				associtivity = atoi (argv[i + 1]);
			} else if (!strcmp(argv[i],"-i")) {
				eviction_policy = atoi (argv[i + 1]);
			}
			/* everything else you need to add here */
		}
	}
	////////////////////////
	//Our Code:
//	Metadata* MemArray = Initialize_Memory(mem_size, init_policy);
	Initialize_Memory(mem_size, init_policy);
	Instruction inst(DATA_GARBAGE, DATA_GARBAGE, DATA_GARBAGE);
	Cache cache(cache_size, block_size, associtivity, eviction_policy, init_policy);
	////////////////////////

	fr = fopen (inst_filename, "rt");  /* open the file for reading */

	if (fr == NULL) {
		perror ("Error opening file");
		return -1;//FIXME remove
	}

	// scans the program file, reads 1 instruction at a time
	while(fgets(line, 80, fr) != NULL)
	{
	 /* split tokens, put back into inst */
	 inst = split_inst(line);
	 if (inst.ID() == STORE_COM) {
		 cache.StoreInstruction(inst);
	 } else {
		 //TODO Load instruction
	 }

	 /* now you  should have the instruction decoded in "inst" */
	 // try to remove the next remarks and see that it works
	 printf("seperated tokens; inst_id: %d , inst_addr: %d , inst_data: %d \n",inst.ID(), inst.address(), inst.data());
	 // printf("%x %X\n",inst->data,inst->data);
	 /* Do whatever you want here. For example, a cache simulator! */



	 }
	cache.PrintAllCache();

	fclose(fr);  /* close the file prior to exiting the routine */


	return 0;
}



