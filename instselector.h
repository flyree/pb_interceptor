#ifndef INST_SELECTOR_H
#define INST_SELECTOR_H

#include "pin.H"

void configInstSelector();

bool isInstFITarget(INS ins);

extern KNOB<string> instcount_file;

#endif
