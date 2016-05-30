//
// Created by Bo Fang on 2016-05-18.
//

#include<iostream>
#include<fstream>

#include <set>
#include <map>
//#include <string>

#include "pin.H"
#include "utils.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

KNOB<UINT64> randInst(KNOB_MODE_WRITEONCE, "pintool",
                      "randinst","0", "random instructions");

KNOB<string> pcfile(KNOB_MODE_WRITEONCE, "pintool",
                    "pcfile","pcfile", "file name that stores ");

static UINT64 allinst = 0;


VOID printip(void *ip){
    ofstream OutFile;
    OutFile.open(pcfile.Value().c_str());
    OutFile << ip << endl;
    OutFile.close();
}
// Pin calls this function every time a new instruction is encountered
VOID CountInst(INS ins, VOID *v)
{
    allinst++;
    if (randInst.Value() == allinst){
        cout << INS_Disassemble(ins) << endl;
        INS_InsertCall(ins,IPOINT_BEFORE,(AFUNPTR)printip, IARG_INST_PTR, IARG_END);
        REG reg;
        if (INS_IsMemoryWrite(ins) || INS_IsMemoryRead(ins)) {
            REG reg = INS_MemoryBaseReg(ins);
            cout <<"reg0:" + REG_StringShort(reg) << endl;
            if (!REG_valid(reg)) {
                reg = INS_MemoryIndexReg(ins);
                cout <<"reg1:" + REG_StringShort(reg) << endl;
            }

        }
        else {
            int numW = INS_MaxNumWRegs(ins), randW = 0;
            if (numW > 1)
                randW = rand() % numW;
            else
                randW = 0;
            reg = INS_RegW(ins, randW);

            if (numW > 1 && (reg == REG_RFLAGS || reg == REG_FLAGS || reg == REG_EFLAGS))
                randW = (randW + 1) % numW;
            if (numW > 1 && REG_valid(INS_RegW(ins, randW)))
                reg = INS_RegW(ins, randW);
            else
                reg = INS_RegW(ins, 0);
            if (!REG_valid(reg)) {

                cout << "REGNOTVALID: inst " + INS_Disassemble(ins) << endl;
                return;
            }
            if (reg == REG_RFLAGS || reg == REG_FLAGS || reg == REG_EFLAGS) {
                cout << "REGNOTVALID: inst " + INS_Disassemble(ins) << endl;
                return;
            }
            cout <<"reg3:" + REG_StringShort(reg) << endl;
        }
        cout <<"reg:" + REG_StringShort(reg) << endl;
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
