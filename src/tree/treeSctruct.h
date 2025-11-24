#ifndef TREE_TREESCTRUCT_H
#define TREE_TREESCTRUCT_H
#include "common.h"



treeNode_t* createValue(int value, treeNode* left, treeNode* right);
treeNode_t* createOperation(nodeOperation_t operation, treeNode* left, treeNode* right);
treeNode_t* createParameter(char param, treeNode* left, treeNode* right);
treeNode_t* createNode(nodeData data, nodeType_t nodeType, treeNode* left, treeNode* right);

int getParameterValue(char param);

treeNode_t* getRight(treeNode_t* node);
void setRight(treeNode_t* node, treeNode_t* right);

treeNode_t* getLeft(treeNode_t* node);
void setLeft(treeNode_t* node, treeNode_t* left);

treeElType_t getData(treeNode_t* node);
void setData(treeNode_t* node, treeElType_t data);

int getNumber(treeNode* node);
char getParameter(treeNode* node);
nodeOperation_t getOperation(treeNode* node);

nodeType_t getNodeType(treeNode_t* node);
void setNodeType(treeNode_t* node, nodeType_t nodeType);

int validateTree(treeNode_t* root);
void destroyTree(treeNode_t* root);


#endif //TREE_TREESCTRUCT_H