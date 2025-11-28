#ifndef DIFFERENTIATOR_DSLOPERATIONS_H
#define DIFFERENTIATOR_DSLOPERATIONS_H
#include "treeSctruct.h"

#define MUL(n1, n2) createOperation(NODE_MUL, n1, n2)
#define ADD(n1, n2) createOperation(NODE_ADD, n1, n2)
#define SUB(n1, n2) createOperation(NODE_SUB, n1, n2)
#define DIV(n1, n2) createOperation(NODE_DIV, n1, n2)

int findTreeValue(treeNode_t* node);

int addFunction(treeNode_t *node);
int subtractionFunction(treeNode_t *node);
int multiplicationFunction(treeNode_t *node);
int divisionFunction(treeNode_t *node);


treeNode_t* differentiate(treeNode_t* node);

treeNode_t* diffAddition(treeNode_t *node);
treeNode_t* diffSubtraction(treeNode_t *node);
treeNode_t* diffMultiplication(treeNode_t *node);
treeNode_t* diffDivision(treeNode_t* node);

double constantsFolding(treeNode_t* node, bool* changed);
int removeRedurantOperations(treeNode_t* node, bool* changed);

void printTree(treeNode_t* node);

typedef struct operationInfo {
    const nodeOperation_t operation;
    const char representation;
    int (*operationFunction)(treeNode_t *node);
    treeNode_t* (*diffFunction)(treeNode_t* node);
} operationInfo_t;

const operationInfo_t DSL_OPERATIONS_INFO[] = {
    {NODE_ADD, '+', addFunction,            diffAddition      },
    {NODE_SUB, '-', subtractionFunction,    diffSubtraction   },
    {NODE_MUL, '*', multiplicationFunction, diffMultiplication},
    {NODE_DIV, '/', divisionFunction,       diffDivision      },
};

const int DSL_OPERATIONS_COUNT = sizeof(DSL_OPERATIONS_INFO) / sizeof(operationInfo_t);

#endif //DIFFERENTIATOR_DSLOPERATIONS_H