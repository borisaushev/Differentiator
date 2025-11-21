#ifndef TREE_TREESCTRUCT_H
#define TREE_TREESCTRUCT_H
#include "common.h"

typedef enum nodeOperation {
    NODE_ADD,
    NODE_SUB,
    NODE_MUL,
    NODE_DIV,
} nodeOperation_t;

typedef enum nodeType {
    OPERATION_TYPE = 0,
    NUMBER_TYPE = 1,
    PARAM_TYPE = 2,
} nodeType_t;

union nodeData {
    int number;
    char parameter;
    nodeOperation_t operation;
};

typedef nodeData treeElType_t;

typedef struct treeNode {
    nodeType_t nodeType;
    treeElType_t data;
    treeNode* left;
    treeNode* right;
} treeNode_t;

typedef enum tree_error {
    TR_SUCCESS = 0,
    TR_NULL_PTR,
    TR_INVALID_PTR,
    TR_FILE_NOT_FOUND,
    TR_INVALID_INPUT,
    TR_INVALID_SIZE,
    TR_CANT_OPEN_FILE
} tree_error_t;

treeNode_t* createValue(int value, nodeType_t nodeType, treeNode* left, treeNode* right);
treeNode_t* createOperation(nodeOperation_t operation, nodeType_t nodeType, treeNode* left, treeNode* right);
treeNode_t* createParameter(char param, nodeType_t nodeType, treeNode* left, treeNode* right);

int getParameterValue(char param);

treeNode_t* getRight(treeNode_t* node);
void setRight(treeNode_t* node, treeNode_t* right);

treeNode_t* getLeft(treeNode_t* node);
void setLeft(treeNode_t* node, treeNode_t* left);

treeElType_t getData(treeNode_t* node);
void setData(treeNode_t* node, treeElType_t data);

nodeType_t getNodeType(treeNode_t* node);
void setNodeType(treeNode_t* node, nodeType_t nodeType);

int validateTree(treeNode_t* root);
void destroyTree(treeNode_t* root);


#endif //TREE_TREESCTRUCT_H