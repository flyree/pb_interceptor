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
extern KNOB<BOOL> is_inlib;
UINT32 parseLibNames(string libfilename);
VOID libLoad(RTN rtn,VOID *v);
const char * stripPath(const char * path);

#endif //PB_INTERCEPTOR_LIBLOAD_H
