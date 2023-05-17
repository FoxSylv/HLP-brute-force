#ifndef HLPLIB
#define HLPLIB

unsigned long long createLayer(unsigned int sideSS, unsigned int backSS, int isSideSMode, int isBackSMode);
unsigned long long composeFunctions(unsigned long long function1, unsigned long long function2);
unsigned int* extractFunctionData(unsigned long long function);
unsigned long long compactFunction(unsigned int* function);

#endif
