#ifndef HLPLIB
#define HLPLIB

typedef unsigned long long HLPFunction;
typedef unsigned long long HLPLayer;

HLPLayer createLayer(unsigned int sideSS, unsigned int backSS, int isSideSMode, int isBackSMode);
HLPFunction composeFunctions(HLPFunction function1, HLPFunction function2);
unsigned int* extractFunctionData(HLPFunction function);
HLPFunction compactFunction(unsigned int* function);

#endif
