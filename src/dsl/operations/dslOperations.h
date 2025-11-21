#ifndef DIFFERENTIATOR_DSLOPERATIONS_H
#define DIFFERENTIATOR_DSLOPERATIONS_H
#include "treeSctruct.h"

int findTreeValue(treeNode_t* node);

treeNode_t* differentiate(treeNode_t* node);

double groupConstants(treeNode_t* node);

void printTree(treeNode_t* node);

#endif //DIFFERENTIATOR_DSLOPERATIONS_H