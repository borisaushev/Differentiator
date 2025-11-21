#ifndef DIFFERENTIATOR_DSLOPERATIONS_H
#define DIFFERENTIATOR_DSLOPERATIONS_H
#include "treeSctruct.h"

int findTreeValue(treeNode_t* node);

treeNode_t* differentiate(treeNode_t* node);

double groupConstants(treeNode_t* node);

void printTree(treeNode_t* node);


#define MUL(n1, n2) createOperation(NODE_MUL, n1, n2)
#define ADD(n1, n2) createOperation(NODE_ADD, n1, n2)
#define SUB(n1, n2) createOperation(NODE_SUB, n1, n2)

#define dL

#endif //DIFFERENTIATOR_DSLOPERATIONS_H