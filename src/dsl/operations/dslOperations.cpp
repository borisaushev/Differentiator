#include "dslOperations.h"

#include "treeDump.h"
#include "treeSctruct.h"

int findTreeValue(treeNode_t* node) {
    if (node == NULL) {
        PRINTERR("NULL NODE");
        return 0;
    }
    switch (getNodeType(node)) {
        case NUMBER_TYPE: {
            return getData(node).number;
        }
        case PARAM_TYPE: {
            int value = getParameterValue(getData(node).parameter);
            return value;
        }
        case OPERATION_TYPE: {
            switch (getData(node).parameter) {
                case NODE_ADD: {
                    return findTreeValue(getLeft(node)) + findTreeValue(getRight(node));
                }
                case NODE_SUB: {
                    return findTreeValue(getLeft(node)) - findTreeValue(getRight(node));
                }
                case NODE_MUL: {
                    return findTreeValue(getLeft(node)) * findTreeValue(getRight(node));
                }
                case NODE_DIV: {
                    return findTreeValue(getLeft(node)) / findTreeValue(getRight(node));
                }
                default: {
                    PRINTERR("invalid operation");
                    return 0;
                }
            }
            default: {
                PRINTERR("invalid node type");
                return 0;
            }
        }
    }

    return 0;
}

treeNode* copyTree(treeNode_t* node) {
    if (node == NULL) {
        return NULL;
    }
    return createNode(getData(node), copyTree(getLeft(node)), copyTree(getRight(node)));
}

treeNode_t* differentiate(treeNode_t* node) {
    switch (getNodeType(node)) {
        case NUMBER_TYPE: {
            setData(node, {0});
            return node;
        }
        case OPERATION_TYPE: {
            switch (getData(node).operation) {
                case NODE_ADD: {
                    differentiate(getLeft(node));
                    differentiate(getRight(node));
                    return node;
                }
                case NODE_SUB: {
                    differentiate(getLeft(node));
                    differentiate(getRight(node));
                    return node;
                }
                case NODE_MUL: {
                    setData(node, {NODE_ADD});
                    treeNode_t* left = getLeft(node);
                    treeNode_t* right = getRight(node);

                    treeNode_t* leftCopy = copyTree(left);
                    treeNode_t* rightCopy = copyTree(right);

                    return createOperation(NODE_ADD,
                                           createOperation(NODE_MUL, differentiate(left), rightCopy),
                                           createOperation(NODE_MUL, differentiate(right), leftCopy));
                }
                default: {
                    PRINTERR("invalid operation");
                    return NULL;
                }
            }
        }
        case PARAM_TYPE: {
            setData(node, {getData(node).parameter == 'x' ? 1 : 0});
            setNodeType(node, NUMBER_TYPE);
            return node;
        }
        default: {
            PRINTERR("invalid node type");
            return NULL;
        }
    }
}
