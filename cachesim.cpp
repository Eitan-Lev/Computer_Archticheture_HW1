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

#define DEFAULT_MEM_SIZE 1024

char line[80];
char* inst_filename;
int mem_size=DEFAULT_MEM_SIZE;
FILE *fr;

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


int split_inst(char* line, Instruction inst)
{
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
	return 0;
}


int getbits(int data, int high_bound, int low_bound)
{
	int tempval=data;
	if (high_bound < 31) tempval = data % (1<<(high_bound+1));
	tempval = tempval / (1<<low_bound);
	return tempval;
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
			}
			/* everything else you need to add here */
		}
	}


	Instruction inst(DATA_GARBAGE, DATA_GARBAGE, DATA_GARBAGE);
	fr = fopen (inst_filename, "rt");  /* open the file for reading */

	// scans the program file, reads 1 instruction at a time
	while(fgets(line, 80, fr) != NULL)
	{
	 /* split tokens, put back into inst */
	 split_inst(line, inst);

	 /* now you  should have the instruction decoded in "inst" */
	 // try to remove the next remarks and see that it works
	 printf("seperated tokens; inst_id: %d , inst_addr: %d , inst_data: %d \n",inst.ID(), inst.address(), inst.data());
	 // printf("%x %X\n",inst->data,inst->data);
	 /* Do whatever you want here. For example, a cache simulator! */



	 }

	fclose(fr);  /* close the file prior to exiting the routine */


	return 0;
}



