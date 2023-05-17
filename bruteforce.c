#include <stdlib.h>
#include <stdio.h>
#include "hlplib.h"

#define MAX_LAYER_SPACE 1024


/*
 * getUniqueOutputCount -
 *
 * Finds the number of unique ss outputs of the given function.
 */
int getUniqueOutputCount(unsigned long long function) {
    /* Set up counter */
    int ssCount[16];
    for (int i = 0; i < 16; ++i) {
        ssCount[i] = 0;
    }

    /* Count and tally */
    for (int b = 0; b < 64; b += 4) {
        ++ssCount[(function >> b) & 0xF];
    }
    int uniques = 0;
    for (int i = 0; i < 16; ++i) {
        uniques += (ssCount[i] > 0);
    }

    return uniques;
}

/*
 * getAllLayers -
 *
 * Returns an 2d array with all one-layer functions such that the first indice
 * indicates how many unique outputs get removed by applying that function.
 */
unsigned long long** getAllLayers() {
    /* Set up memory */
    unsigned long long** layers = (unsigned long long**)malloc(16 * sizeof(unsigned long long*));
    for (int i = 0; i < 16; ++i) {
        layers[i] = (unsigned long long*)malloc(MAX_LAYER_SPACE * sizeof(unsigned long long));
        for (int l = 0; l < MAX_LAYER_SPACE; ++l) {
            layers[i][l] = 0;
        }
    }
    int layerIndices[16];
    for (int i = 0; i < 16; ++i) {
        layerIndices[i] = 0;
    }

    /* Sort layers by number of unique outputs */
    for (unsigned int sideSS = 0; sideSS < 16; ++sideSS) {
        for (unsigned int backSS = 0; backSS < 16; ++backSS) {
            for (int isSideSMode = 0; isSideSMode < 2; ++isSideSMode) {
                for (int isBackSMode = 0; isBackSMode < 2; ++isBackSMode) {
                    unsigned long long layer = createLayer(sideSS, backSS, isSideSMode, isBackSMode);
                    if (layer != 0 && layer != 0xFEDCBA9876543210) {
                        /* Do not include duplicates */
                        int isDuplicate = 0;
                        int outputsRemoved = 16 - getUniqueOutputCount(layer);
                        for (int l = 0; l < layerIndices[outputsRemoved]; ++l) {
                            if (layers[outputsRemoved][l] == layer) {
                                isDuplicate = 1;
                            }
                        }
                        if (!isDuplicate) {
                            layers[outputsRemoved][layerIndices[outputsRemoved]++] = layer;
                        }
                    }
                }
            }
        }
    }

    return layers;
}



/*
 * findSolutionRecursive -
 *
 * A recursive solution finder that uses depth-first search and
 * backtracking to find solutions. Returns 1 on success and 0 on fail.
 *
 * Note that if a solution is found, it may not necessarily be the most
 * optimal number of layers due to the depth-first search.
 */
int findSolutionRecursive(unsigned long long desiredResult, unsigned long long currentFunction, int uniqueOutputMargin, unsigned long long* solutionStack, int* currentDepth, int maxDepth, unsigned long long** layers) {
    /* Base cases */
    if (currentFunction == desiredResult) {
        return 1;
    }
    if (*currentDepth == maxDepth - 1) {
        return 0;
    }

    /* Function output legality checker */
    for (int x = 0; x < 64; x += 4) {
        for (int y = x; y < 64; y += 4) {
            if (((currentFunction >> x) & 0xF) == ((currentFunction >> y) & 0xF)) {
                if (((desiredResult >> x) & 0xF) != ((desiredResult >> y) & 0xF)) {
                    return 0;
                }
            }
        }
    }

    /* Search one layer deeper */
    ++*currentDepth;
    for (int r = 0; r <= uniqueOutputMargin; ++r) {
        int newMargin = uniqueOutputMargin - r;
        for (unsigned long long* layer = layers[r]; *layer; ++layer) {
            unsigned long long newFunction = composeFunctions(currentFunction, *layer);
            solutionStack[*currentDepth] = *layer;
            if (findSolutionRecursive(desiredResult, newFunction, newMargin, solutionStack, currentDepth, maxDepth, layers)) {
                return 1;
            }
        }
    }
    --*currentDepth;
    return 0;
}


void printSolution(unsigned long long* solutionStack, int stackSize) {
    for (int s = 0; s < stackSize; ++s) {
        printf("%llx\n", solutionStack[s]);
    }
}

/*
 * findSolution -
 *
 * Finds and prints a solution for the given function if one exists in the
 * given number of layers. Otherwise returns NULL.
 */
void findSolution(unsigned long long function, int maxDepth) {
    /* Initialize starting values */
    unsigned long long** layers = getAllLayers();
    unsigned long long* solutionStack = (unsigned long long*)malloc(maxDepth * sizeof(unsigned long long));
    int solutionDepth = -1;

    /* Find and print solution, if it exists */
    int solutionFound = findSolutionRecursive(function, 0xFEDCBA9876543210, 16 - getUniqueOutputCount(function), solutionStack, &solutionDepth, maxDepth, layers);
    if (solutionFound) {
        printSolution(solutionStack, solutionDepth + 1);
    }
    else {
        printf("No solution found in depth %i. Sorry!\n", maxDepth);
    }

    /* Memory clean-up */
    for (int i = 0; i < 16; ++i) {
        free(layers[i]);
    }
    free(layers);
    free(solutionStack);
}


int main(int argc, char** argv) {
    unsigned long long temp = 0x8080808080808080;
    findSolution(temp, 7);
}
