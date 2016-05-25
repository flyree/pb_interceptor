//
// Created by Bo Fang on 2016-05-18.
//

#include<iostream>
#include<fstream>

#include <set>
#include <map>
#include <string>

#include "pin.H"
#include "utils.h"

KNOB<UINT64> randInst(KNOB_MODE_WRITEONCE, "pintool",
                      "randinst","0", "Total number of instructions");

static UINT64 allinst = 0;


VOID printip(void *ip){
    cout << ip << endl;
}
// Pin calls this function every time a new instruction is encountered
VOID CountInst(INS ins, VOID *v)
{
    allinst++;
    if (randInst.Value() == allinst){
        INS_InsertCall(ins,IPOINT_BEFORE,(AFUNPTR)printip, IARG_INST_PTR, IARG_END);
    }
}

// bool mayChangeControlFlow(INS ins){
// 	REG reg;
// 	if(!INS_HasFallThrough(ins))
// 		return true;
// 	int numW = INS_MaxNumWRegs(ins);
// 	for(int i =0; i < numW; i++){
// 		if(reg == REG_RIP || reg == REG_EIP || reg == REG_IP) // conditional branches
// 			return true;
// 	}
// 	return false;
// }
// This function is called when the application exits
VOID Fini(INT32 code, VOID *v)
{
    // Write to a file since cout and cerr maybe closed by the application
    //ofstream OutFile;
    //OutFile.open(instcount_file.Value().c_str());
    //OutFile.setf(ios::showbase);
    //OutFile.close();
}

/* ===================================================================== */
/* Print Help Message                                                    */
/* ===================================================================== */

INT32 Usage()
{
    cerr << "This tool counts the number of dynamic instructions executed" << endl;
    cerr << endl << KNOB_BASE::StringKnobSummary() << endl;
    return -1;
}



int main(int argc, char * argv[])
{
    PIN_InitSymbols();
    // Initialize pin
    if (PIN_Init(argc, argv)) return Usage();

    // Register Instruction to be called to instrument instructions
    INS_AddInstrumentFunction(CountInst, 0);

    // Register Fini to be called when the application exits
    PIN_AddFiniFunction(Fini, 0);

    // Start the program, never returns
    PIN_StartProgram();

    return 0;
}
