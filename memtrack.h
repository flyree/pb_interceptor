//
// Created by pwu on 1/26/16.
//

#ifndef PB_INTERCEPTOR_MEMTRACK_H
#define PB_INTERCEPTOR_MEMTRACK_H
#include <iostream>
#include <map>
#include "pin.H"
extern map<VOID*,UINT64> MemStore;
extern UINT64 NMemWriteInstStatic;
VOID memtrack(INS ins, VOID *);

#endif //PB_INTERCEPTOR_MEMTRACK_H

