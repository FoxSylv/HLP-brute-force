#include <stdlib.h>
#include "hlplib.h"

#define max(x, y) (x < y) ? y : x


/*
 * comparator -
 *
 * Does a Minecraft comparator operation using the given inputs.
 */
unsigned int comparator(unsigned int backSS, unsigned int sideSS, int isSMode) {
    if (sideSS > backSS) {
        return 0;
    }
    return isSMode ? (backSS - sideSS) : backSS;
}


/*
 * createLayer -
 *
 * Creates a new one-layer function from the given comparator data.
 * The "side" comparator data is for the comparator in the layer where the barrel feeds into its side.
 * Hss data is not supported due to the how the function data is compacted.
 */
HLPLayer createLayer(unsigned int sideSS, unsigned int backSS, int isSideSMode, int isBackSMode) {
    HLPLayer layer = 0;
    for (int ss = 0; ss < 16; ++ss) {
        unsigned int backOutput = comparator(backSS, ss, isBackSMode);
        unsigned int sideOutput = comparator(ss, sideSS, isSideSMode);
        unsigned int outputSS = max(backOutput, sideOutput);
        layer |= ((HLPLayer)outputSS) << (ss << 2);
    }
    return layer;
}

/*
 * composeFunctions -
 *
 * Takes two functions in compact form and chains them together.
 */
HLPFunction composeFunctions(HLPFunction function1, HLPFunction function2) {
    HLPFunction output = 0; 
    for (int b = 0; b < 64; b += 4) {
        unsigned long long midSS = (function1 >> b) & 0xF;
        output |= ((function2 >> (midSS << 2)) & 0xF) << b;
    }
    return output;
}


/*
 * extractFunctionData -
 *
 * Takes a function in compact form and returns an array where indices
 * denote ss input and values indicate ss output.
 */
unsigned int* extractFunctionData(HLPFunction function) {
    unsigned int* data = (unsigned int*)malloc(16 * sizeof(unsigned int));
    for (int ss = 0; ss < 16; ++ss) {
        data[ss] = (function >> (ss << 2)) & 0xF;
    }
    return data;
}

/*
 * compactFunction -
 *
 * Takes an array where indices denote ss input and values indicate ss output 
 * and returns the corresponding function in compact form.
 * Hss data is not supported due to the how the function data is compacted.
 */
HLPFunction compactFunction(unsigned int* function) {
    HLPFunction output = 0;
    for (int ss = 0; ss < 16; ++ss) {
        output |= (function[ss]) << (ss << 2);
    }
    return output;
}
