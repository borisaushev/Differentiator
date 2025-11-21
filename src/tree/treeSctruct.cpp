#include "treeSctruct.h"

#include <cassert>
#include <cstdio>

#include "common.h"
#include "treeDump.h"

static int parameter = 0;

treeNode_t* getRight(treeNode_t* node) {
    assert(node);
    return node->right;
}

void setRight(treeNode_t* node, treeNode_t* right) {
    assert(node);
    node->right = right;
}

treeNode_t* getLeft(treeNode_t* node) {
    assert(node);
    return node->left;
}

void setLeft(treeNode_t* node, treeNode_t* left) {
    assert(node);
    node->left = left;
}

treeElType_t getData(treeNode_t* node) {
    assert(node);
    return node->data;
}


void setData(treeNode_t* node, treeElType_t data) {
    assert(node);

    node->data = data;
}

nodeType_t getNodeType(treeNode_t* node) {
    assert(node);
    return node->nodeType;
}

void setNodeType(treeNode_t* node, nodeType_t nodeType) {
    assert(node);
    node->nodeType = nodeType;
}

int getParameterValue(char param) {
    return parameter;
}

treeNode_t* createParameter(char param, treeNode* left, treeNode* right) {
    treeNode_t* result = (treeNode_t*)calloc(1, sizeof(treeNode_t));

    if (parameter == 0) {
        printf("what's the value of parameter '%c'?  ", param);
        if (scanf("%d", &parameter) != 1) {
            PRINTERR("invalid parameter input");
            return NULL;
        }
    }
    printf("\n");
    result->nodeType = PARAM_TYPE;
    result->left = left;
    result->right = right;

    result->data = {param};

    return result;
}

treeNode_t* createValue(int value, treeNode* left, treeNode* right) {
    treeNode_t* result = (treeNode_t*)calloc(1, sizeof(treeNode_t));
    result->nodeType = NUMBER_TYPE;
    result->left = left;
    result->right = right;

    result->data = {value};

    return result;
}

treeNode_t* createOperation(nodeOperation_t operation, treeNode* left, treeNode* right) {
    treeNode_t* result = (treeNode_t*)calloc(1, sizeof(treeNode_t));
    result->nodeType = OPERATION_TYPE;
    result->left = left;
    result->right = right;

    result->data = {operation};

    return result;
}

treeNode_t* createNode(nodeData data, treeNode* left, treeNode* right) {
    treeNode_t* result = (treeNode_t*)calloc(1, sizeof(treeNode_t));
    result->nodeType = OPERATION_TYPE;
    result->left = left;
    result->right = right;

    result->data = data;

    return result;
}


static void countTreeSize(treeNode_t* node, size_t* size) {
    if ((*size)++ > MAX_TREE_DEPTH) {
        return;
    }

    if (getLeft(node) != NULL) {
        countTreeSize(getLeft(node), size);
    }
    if (getRight(node) != NULL) {
        countTreeSize(getRight(node), size);
    }
}

int validateTree(treeNode_t* root) {
    if (root == NULL) {
        RETURN_ERR(TR_NULL_PTR, "root is null");
    }

    size_t size = 0;
    countTreeSize(root, &size);
    if (size > MAX_TREE_DEPTH) {
        RETURN_ERR(TR_INVALID_SIZE, "tree is cycled");
    }

    return TR_SUCCESS;
}


static void destroyNode(treeNode_t* node) {
    if (node != NULL) {
        destroyNode(getLeft(node));
        destroyNode(getRight(node));
        free(node);
    }
}

int destroyLeftNode(treeNode* parentNode) {
    destroyNode(getLeft(parentNode));
    setLeft(parentNode, NULL);

    return TR_SUCCESS;
}

int destroyRightNode(treeNode* parentNode) {
    destroyNode(getRight(parentNode));
    setRight(parentNode, NULL);

    return TR_SUCCESS;
}

void destroyTree(treeNode_t* root) {
    destroyNode(root);
}

