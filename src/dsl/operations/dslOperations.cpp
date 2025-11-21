#include "dslOperations.h"

#include "treeDump.h"
#include "treeSctruct.h"

int executeTree(treeNode_t* node) {
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
                    return executeTree(getLeft(node)) + executeTree(getRight(node));
                }
                case NODE_SUB: {
                    return executeTree(getLeft(node)) - executeTree(getRight(node));
                }
                case NODE_MUL: {
                    return executeTree(getLeft(node)) * executeTree(getRight(node));
                }
                case NODE_DIV: {
                    return executeTree(getLeft(node)) / executeTree(getRight(node));
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

treeNode* copyTree(treeNode_t node) {
    return NULL; //TODO
}

treeNode* differentiate(treeNode_t* node) {
    switch (getNodeType(node)) {
        case NUMBER_TYPE: {
            return 0;
        }
        case OPERATION_TYPE: {
            switch (getData(node).operation) {
                case NODE_ADD: {
                    differentiate(getLeft(node));
                    differentiate(getRight(node));
                    break;
                }
                case NODE_SUB: {
                    differentiate(getLeft(node));
                    differentiate(getRight(node));
                    break;
                }
                case NODE_MUL: {
                     setData(node, {NODE_ADD});

                }
            }
        }
    }
}
