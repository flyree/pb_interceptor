//
// Created by Bo Fang on 2016-02-25.
//

#ifndef PB_INTERCEPTOR_LIBLOAD_H
#define PB_INTERCEPTOR_LIBLOAD_H

#include "pin.H"
#include <string>
#include <fstream>
#include <iostream>

extern std::vector<std::string> libs;
extern KNOB<string> libnames;
VOID parseLibNames(string libfilename)
VOID libLoad(RTN rtn,VOID *v);

#endif //PB_INTERCEPTOR_LIBLOAD_H
