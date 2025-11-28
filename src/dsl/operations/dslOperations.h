#ifndef DIFFERENTIATOR_DSLOPERATIONS_H
#define DIFFERENTIATOR_DSLOPERATIONS_H
#include "treeSctruct.h"

#define MUL(n1, n2) createOperation(NODE_MUL, n1, n2)
#define ADD(n1, n2) createOperation(NODE_ADD, n1, n2)
#define SUB(n1, n2) createOperation(NODE_SUB, n1, n2)
#define DIV(n1, n2) createOperation(NODE_DIV, n1, n2)
#define COS(f)      createOperation(NODE_COS, f,  NULL)
#define NEG(f)      MUL(f, createNumber(-1))

double findTreeValue(treeNode_t* node);

double addFunction(treeNode_t *node);
double subtractionFunction(treeNode_t *node);
double multiplicationFunction(treeNode_t *node);
double divisionFunction(treeNode_t *node);
double sinFunction(treeNode_t *node);
double cosFunction(treeNode_t *node);

treeNode_t* differentiate(treeNode_t* node);

treeNode_t* diffAddition(treeNode_t *node);
treeNode_t* diffSubtraction(treeNode_t *node);
treeNode_t* diffMultiplication(treeNode_t *node);
treeNode_t* diffDivision(treeNode_t* node);
treeNode_t* diffCos(treeNode_t* node);
treeNode_t* diffSin(treeNode_t* node);

double constantsFolding(treeNode_t* node, bool* changed);
int removeRedurantOperations(treeNode_t* node, bool* changed);

void printTree(treeNode_t* node);

typedef struct operationInfo {
    const nodeOperation_t operation;
    const char* const representation;
    double (*operationFunction)(treeNode_t *node);
    treeNode_t* (*diffFunction)(treeNode_t* node);
    bool isAFunction;
} operationInfo_t;

const operationInfo_t DSL_OPERATIONS_INFO[] = {
    {NODE_ADD, "+",   addFunction,            diffAddition,       false},
    {NODE_SUB, "-",   subtractionFunction,    diffSubtraction,    false},
    {NODE_MUL, "*",   multiplicationFunction, diffMultiplication, false},
    {NODE_DIV, "/",   divisionFunction,       diffDivision,       false},
    {NODE_SIN, "sin", sinFunction,            diffSin,            true},
    {NODE_COS, "cos", cosFunction,            diffCos,            true},
};

const int DSL_OPERATIONS_COUNT = sizeof(DSL_OPERATIONS_INFO) / sizeof(operationInfo_t);

#endif //DIFFERENTIATOR_DSLOPERATIONS_H