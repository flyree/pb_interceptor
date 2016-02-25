//
// Created by Bo Fang on 2016-02-24.
//

#include "pin.H"
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include "faultinjection.h"

using namespace std;

KNOB<string> libnames(KNOB_MODE_WRITEONCE, "pintool",
                               "libload", "libnames", "to be loaded lib files");


const char * StripPath(const char * path)
{
    const char * file = strrchr(path,'/');
    if (file)
        return file+1;
    else
        return path;
}

/*----
 *  Check library images based on user input, and instrument each instruction inside that routine
 */

VOID libLoad(RTN rtn,VOID *v)
{
    ifstream infile(libnames.Value());
    string line;
    while (getline(infile,line))
    {
       string image = StripPath(IMG_NAME(SEC_Img(Rtn_Sec(rtn))));
       if (image.find(line) != string::npos)
       {
           RTN_Open(rtn);
           for (INS ins = RTN_InsHead(rtn); INS_Valid(ins); ins = INS_NEXT(ins))
           {
               instruction_Instrumentation(ins,v);
           }
           RTN_Close(rtn);
       }
    }
}