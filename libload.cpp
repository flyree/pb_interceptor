//
// Created by Bo Fang on 2016-02-24.
//

#include "pin.H"
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include "faultinjection.h"
#include <string.h>
#include <vector>

using namespace std;

KNOB<string> libnames(KNOB_MODE_WRITEONCE, "pintool",
                               "libload", "libnames", "to be loaded lib files");
KNOB<BOOL> is_inlib(KNOB_MODE_WRITEONCE, "pintool",
                      "isinlib","", "if we need to only look at instructions in a lib");

vector<string> libs;

UINT32 parseLibNames(string libfilename)
{
    string line;
    ifstream libfile(libfilename.c_str());
    if (!libfile)
    {
        cout << "Error opening lib files!" << endl;
        return -1;
    }
    while (getline(libfile,line))
    {
        libs.push_back(line);
    }
}


const char * stripPath(const char * path)
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
    for (vector<string>::iterator it = libs.begin(); it != libs.end(); ++it)
    {
       string image = stripPath(IMG_Name(SEC_Img(RTN_Sec(rtn))).c_str());
       if (image.find(*it) != string::npos)
       {
           RTN_Open(rtn);
           for (INS ins = RTN_InsHead(rtn); INS_Valid(ins); ins = INS_Next(ins))
           {
               instruction_Instrumentation(ins,v);
           }
           RTN_Close(rtn);
       }
    }
}