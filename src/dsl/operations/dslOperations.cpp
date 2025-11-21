#include "dslOperations.h"

#include "dslInput.h"
#include "treeDump.h"
#include "treeSctruct.h"

int findTreeValue(treeNode_t* node) {
    if (node == NULL) {
        PRINTERR("NULL NODE");
        return 0;
    }
    treeLog("Finding tree value");
    TREE_DUMP(node, "findTreeValue", DSL_SUCCESS);

    //tex dump
    //optimization
    //variables
    //TODO d(pow)
    //cleanup
    switch (getNodeType(node)) {
        case NUMBER_TYPE: {
            treeLog("returning number: %d", getData(node).number);
            return getData(node).number;
        }
        case PARAM_TYPE: {
            int value = getParameterValue(getData(node).parameter);
            treeLog("Returning parameter: %c = %d", getData(node).parameter, value);
            return value;
        }
        case OPERATION_TYPE: {
            int leftValue = findTreeValue(getLeft(node));
            int rightValue = findTreeValue(getRight(node));
            switch (getData(node).parameter) {
                case NODE_ADD: {
                    treeLog("adding trees");
                    int value = leftValue + rightValue;

                    logTex("\\begin{math}\n");
                    texLogRec(node);
                    logTex(" = %d + %d = %d", leftValue, rightValue, value);
                    logTex("\n\\end{math}\n\n");

                    treeLog("Add result: %d", value);
                    TREE_DUMP(node, "After add", DSL_SUCCESS);
                    return value;
                }
                case NODE_SUB: {
                    treeLog("subtracting trees");
                    int value =  leftValue - rightValue;

                    logTex("\\begin{math}\n");
                    texLogRec(node);
                    logTex(" = %d - %d = %d", leftValue, rightValue, value);
                    logTex("\n\\end{math}\n\n");

                    treeLog("Sub result: %d", value);
                    TREE_DUMP(node, "After sub", DSL_SUCCESS);
                    return value;
                }
                case NODE_MUL: {
                    treeLog("multiplying trees");
                    int value =  leftValue * rightValue;

                    logTex("\\begin{math}\n");
                    texLogRec(node);
                    logTex(" = %d * %d = %d", leftValue, rightValue, value);
                    logTex("\n\\end{math}\n\n");

                    treeLog("Mul result: %d", value);
                    TREE_DUMP(node, "After mul", DSL_SUCCESS);
                    return value;
                }
                case NODE_DIV: {
                    treeLog("dividing trees");
                    int value =  leftValue / rightValue;

                    logTex("\\begin{math}\n");
                    texLogRec(node);
                    logTex(" = \\fraq{%d}{%d} = %d", leftValue, rightValue, value);
                    logTex("\n\\end{math}\n\n");

                    treeLog("Div result: %d", value);
                    TREE_DUMP(node, "After div", DSL_SUCCESS);
                    return value;
                }
                default: {
                    PRINTERR("invalid operation");
                    return 0;
                }
            }
        }
        default: {
            PRINTERR("invalid node type");
            return 0;
        }
    }

    return 0;
}

treeNode* copyTree(treeNode_t* node) {
    if (node == NULL) {
        return NULL;
    }
    return createNode(getData(node), getNodeType(node), copyTree(getLeft(node)), copyTree(getRight(node)));
}

treeNode_t* differentiate(treeNode_t* node) {
    TREE_DUMP(node, "Differentiating tree", DSL_SUCCESS);
    switch (getNodeType(node)) {
        case NUMBER_TYPE: {
            setData(node, {0});
            return node;
        }
        case OPERATION_TYPE: {
            switch (getData(node).operation) {
                case NODE_ADD: {
                    treeLog("Differentiating addition");
                    differentiate(getLeft(node));
                    TREE_DUMP(node, "after differentiating left tree", DSL_SUCCESS);

                    treeLog("Differentiating left tree");
                    differentiate(getRight(node));
                    TREE_DUMP(node, "after differentiating right tree", DSL_SUCCESS);

                    return node;
                }
                case NODE_SUB: {
                    treeLog("Differentiating subtraction");
                    differentiate(getLeft(node));
                    TREE_DUMP(node, "after differentiating left tree", DSL_SUCCESS);

                    treeLog("Differentiating left tree");
                    differentiate(getRight(node));
                    TREE_DUMP(node, "after differentiating right tree", DSL_SUCCESS);
                    return node;
                }
                case NODE_MUL: {
                    treeLog("Differentiating multiplication");
                    setData(node, {NODE_ADD});
                    treeNode_t* left = getLeft(node);
                    treeNode_t* right = getRight(node);

                    treeNode_t* leftCopy = copyTree(left);
                    TREE_DUMP(leftCopy, "copied left tree", DSL_SUCCESS);
                    treeNode_t* rightCopy = copyTree(right);
                    TREE_DUMP(rightCopy, "copied right tree", DSL_SUCCESS);


                    treeNode_t * dLeft = differentiate(left);
                    TREE_DUMP(dLeft, "d/dx Left tree", DSL_SUCCESS);

                    treeNode_t * dRight = differentiate(right);
                    TREE_DUMP(dRight, "d/dx Right tree", DSL_SUCCESS);

                    treeNode_t* result = createOperation(NODE_ADD,
                                                         createOperation(NODE_MUL, dLeft, rightCopy),
                                                         createOperation(NODE_MUL, dRight, leftCopy));

                    TREE_DUMP(result, "new operation", DSL_SUCCESS);
                    return result;
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

void printTree(treeNode_t* node) {
    if (node == NULL) {
        return;
    }
    switch (getNodeType(node)) {
        case NUMBER_TYPE: {
            printf("%d", getData(node).number);
            return;
        }
        case OPERATION_TYPE: {
            printf("(");
            printTree(getLeft(node));

            printf(" %s ", DSL_OPERATIONS_INFO[getData(node).operation].representation);

            printTree(getRight(node));
            printf(")");
        }
        case PARAM_TYPE: {
            printf("%c", getData(node).parameter);
            return;
        }
        default: {
            PRINTERR("invalid node type");
        }
    }
}

double groupConstants(treeNode_t* node) {
    if (node == NULL) {
        return NAN;
    }

    switch (getNodeType(node)) {
        case NUMBER_TYPE: {
            return getData(node).number;
        }
        case PARAM_TYPE: {
            return NAN;
        }
        case OPERATION_TYPE: {
            double leftValue = groupConstants(getLeft(node));
            double rightValue = groupConstants(getRight(node));
            if (isnan(leftValue) || isnan(rightValue)) {
                return NAN;
            }

            destroyTree(getLeft(node));
            setLeft(node, NULL);
            destroyTree(getRight(node));
            setRight(node, NULL);

            setNodeType(node, NUMBER_TYPE);
            int sum = (int) (leftValue + rightValue);
            setData(node, {sum});
            return leftValue + rightValue;
        }
    }
    return NAN;
}