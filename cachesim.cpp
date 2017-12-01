/*
 * cachesim.cpp
 *
 *  Created on: 29 ���� 2017
 *      Author: Eitan Levin
 */

#include <string>
#include <cstring>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include "Instruction.h"
#include "Metadata.h"
#include "Cache.h"

using namespace std;

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
Instruction split_inst(char* line);

// Returns value which is located on specific bits of "data": data[high_bound:low_bound]
int getbits(int data, int high_bound, int low_bound);


int HexToDecimal (char* HexNumber)
{
	char* pStopString;
	return (int) strtol (HexNumber, &pStopString, 16);
}

string DecimalToHex(int to_convert){
    std::string result;
    std::stringstream ss;
    ss << std::hex << to_convert;
    ss >> result;
    return result;
}

string IntToString(double to_convert){
    std::string result;
    std::stringstream ss;
    ss << std::dec << to_convert;
    ss >> result;
    return result;
}

string ToUpperCase(string str) {
	const char* tmp = str.c_str();
	char* newStr = new char[str.size()];
	strcpy(newStr, tmp);
	for(unsigned int i = 0; i < str.size(); i++)	{
		newStr[i] = toupper(newStr[i]);
	}
	str = newStr;
	delete newStr;//FIXME maby crazy delete
	return str;
}

Instruction split_inst(char* line)
{
	Instruction inst(DATA_GARBAGE, DATA_GARBAGE, DATA_GARBAGE);
	char* tokens;
	int token_id = 0;
	tokens = strtok (line," \n");
	while (tokens != NULL)
	{
		switch (token_id) {
			case 0: inst._inst_id = (!strcmp(tokens,"store")); // 1 if store, else 0
					break;
			case 1: inst._addr = HexToDecimal(tokens); // store addr//FIXME
					break;
			case 2: inst._data = HexToDecimal(tokens); // store data
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
		MemArray[i]._address = i;
		MemArray[i]._data = init_value;
	}
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
			} else if (!strcmp(argv[i],"-e")) {
				eviction_policy = atoi (argv[i + 1]);
			} else if (!strcmp(argv[i],"-i")) {
				init_policy = atoi (argv[i + 1]);
			}
			/* everything else you need to add here */
		}
	}
	////////////////////////
	//Our Code:
	 stringstream ss;
	srand(time(NULL));
	Initialize_Memory(mem_size, init_policy);
	Instruction inst(DATA_GARBAGE, DATA_GARBAGE, DATA_GARBAGE);
	Cache cache(cache_size, block_size, associtivity, eviction_policy, init_policy);
	////////////////////////
	fr = fopen (inst_filename, "rt");  /* open the file for reading */
	std::ofstream instructionFile("C:\\Users\\Eitan Levin\\Documents\\inst.log");
	std::ofstream statsFile("C:\\Users\\Eitan Levin\\Documents\\stat.log");
	//TODO change later

	if (fr == NULL) {//FIXME
			perror ("Error opening file");
			return -1;//FIXME remove
		}
	double commandIndex = 0;
	double loadHitTimes = 0;
	double loadMissTimes = 0;
	double storeHitTimes = 0;
	double storeMissTimes = 0;
	char* index = new char;
	string CommandLine;
	string CommandResult;
	// scans the program file, reads 1 instruction at a time
	while(fgets(line, 80, fr) != NULL)
	{
		itoa(commandIndex, index, 10);
		CommandLine = index;
		CommandLine += " ";
		CommandResult = index;
		CommandResult += " ";
		CommandLine += line;
		/* split tokens, put back into inst */
		inst = split_inst(line);
		cout << "seperated tokens; inst_id: " << inst.ID() << ", inst_addr: " << inst.Address() << ", inst_data: " << inst.Data() << endl;
		if (inst.ID() == STORE_COM) {
			 bool storeRes = cache.StoreInstruction(inst);
			 CommandResult += "store ";
			 if (storeRes == HIT) {
				 CommandResult += "hit";
				 storeHitTimes++;
			 } else {
				 CommandResult += "miss";
				 storeMissTimes++;
			 }
			 CommandResult += "\n";
		} else {
			 LoadResult loadRes = cache.LoadInstruction(inst);
			 CommandResult += "load ";
			 if (loadRes.CacheStatus() == HIT) {
				 CommandResult += "hit";
				 loadHitTimes++;
			 } else {
				 CommandResult += "miss";
				 loadMissTimes++;
			 }
			 CommandResult += " ";
			 ss << hex << loadRes.LoadData();
			 string data ( ss.str() );
			 ss.str("");
			 //string data = std::to_string(loadRes.LoadData());
//			 string data = DecimalToHex(loadRes.LoadData());
			 data = ToUpperCase(data);
			 CommandResult += data;
			 CommandResult += "\n";
			 //delete[] data;
		}
		 /* now you  should have the instruction decoded in "inst" */
		 // try to remove the next remarks and see that it works
//		 printf("seperated tokens; inst_id: %d , inst_addr: %d , inst_data: %d \n",inst.ID(), inst.Address(), inst.Data());
		 // printf("%x %X\n",inst->data,inst->data);
		 /* Do whatever you want here. For example, a cache simulator! */
		instructionFile << CommandLine;
		instructionFile << CommandResult;
		 commandIndex++;
	 }
	//cache.PrintAllCache();
	string IC = "Instruction Count: " + IntToString(commandIndex);
	string lHits = "\nLoad Hits: " + IntToString(loadHitTimes);
	string lMiss = "\nLoad Misses: " + IntToString(loadMissTimes);
	string sHits = "\nStore Hits: " + IntToString(storeHitTimes);
	string sMiss = "\nStore Misses: " + IntToString(storeMissTimes);
	string lMissRate = "\nLoad Miss Rate: " + IntToString(loadMissTimes*100/(loadMissTimes + loadHitTimes)) + "%";
	string sMissRate = "\nStore Miss Rate: " + IntToString(storeMissTimes*100/(storeMissTimes + storeHitTimes)) + "%";
	string totalMissRate = "\nTotal Miss Rate: " + IntToString(((loadMissTimes + storeMissTimes)*100)/(commandIndex)) + "%\n";
	statsFile << IC;
	statsFile << lHits;
	statsFile << lMiss;
	statsFile << sHits;
	statsFile << sMiss;
	statsFile << lMissRate;
	statsFile << sMissRate;
	statsFile << totalMissRate;
//	delete[] MemArray;//TODO check if we need to delete
	delete index;
	statsFile.close();
	instructionFile.close();

	fclose(fr);  /* close the file prior to exiting the routine */


	return 0;
}



