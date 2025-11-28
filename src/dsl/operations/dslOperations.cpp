#include "dslOperations.h"

#include "dslInput.h"
#include "treeDump.h"
#include "treeSctruct.h"

int addFunction(treeNode_t *node) {
    int leftValue = findTreeValue(getLeft(node));
    int rightValue = findTreeValue(getRight(node));
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

int subtractionFunction(treeNode_t *node) {
    int leftValue = findTreeValue(getLeft(node));
    int rightValue = findTreeValue(getRight(node));

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

int multiplicationFunction(treeNode_t *node) {
    int leftValue = findTreeValue(getLeft(node));
    int rightValue = findTreeValue(getRight(node));
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

int divisionFunction(treeNode_t *node) {
    int leftValue = findTreeValue(getLeft(node));
    int rightValue = findTreeValue(getRight(node));
    treeLog("dividing trees");
    int value =  leftValue / rightValue;

    logTex("\\begin{math}\n");
    texLogRec(node);
    logTex(" = \\frac{%d}{%d} = %d", leftValue, rightValue, value);
    logTex("\n\\end{math}\n\n");

    treeLog("Div result: %d", value);
    TREE_DUMP(node, "After div", DSL_SUCCESS);
    return value;
}

int findTreeValue(treeNode_t* node) {
    if (node == NULL) {
        PRINTERR("NULL NODE");
        return 0;
    }
    treeLog("Finding tree value");
    TREE_DUMP(node, "findTreeValue", DSL_SUCCESS);
    switch (getNodeType(node)) {
        case NUMBER_TYPE: {
            treeLog("returning number: %d", getNumber(node));
            return getData(node).number;
        }
        case PARAM_TYPE: {
            int value = getParameterValue(getData(node).parameter);
            treeLog("Returning parameter: %c = %d", getData(node).parameter, value);
            return value;
        }
        case OPERATION_TYPE: {
            return DSL_OPERATIONS_INFO[getOperation(node)].operationFunction(node);
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

treeNode_t* diffAddition(treeNode_t* node) {
    treeLog("Differentiating addition");
    setLeft(node, differentiate(getLeft(node)));
    TREE_DUMP(node, "after differentiating left tree", DSL_SUCCESS);

    treeLog("Differentiating right tree");
    setRight(node, differentiate(getRight(node)));
    TREE_DUMP(node, "after differentiating right tree", DSL_SUCCESS);
    return node;
}

treeNode_t* diffSubtraction(treeNode_t* node) {
    treeLog("Differentiating subtraction");
    setLeft(node, differentiate(getLeft(node)));
    TREE_DUMP(node, "after differentiating left tree", DSL_SUCCESS);

    setLeft(node, differentiate(getRight(node)));
    TREE_DUMP(node, "after differentiating right tree", DSL_SUCCESS);
    return node;
}

treeNode_t* diffMultiplication(treeNode_t* node) {
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

    treeNode_t* result = ADD(MUL(dLeft, rightCopy), MUL(dRight, leftCopy));

    TREE_DUMP(result, "new operation", DSL_SUCCESS);
    return result;
}

treeNode_t* diffDivision(treeNode_t* node) {
    treeLog("Differentiating division");
    setData(node, {NODE_DIV});
    treeNode_t* left = getLeft(node);
    treeNode_t* right = getRight(node);

    treeNode_t* leftCopy = copyTree(left);
    TREE_DUMP(leftCopy, "copied left tree", DSL_SUCCESS);
    treeNode_t* rightCopy = copyTree(right);
    TREE_DUMP(leftCopy, "copied right tree", DSL_SUCCESS);

    treeNode_t* dLeft = differentiate(left);
    TREE_DUMP(dLeft, "d/dx Left tree", DSL_SUCCESS);

    treeNode_t* dRight = differentiate(right);
    TREE_DUMP(dRight, "d/dx Right tree", DSL_SUCCESS);

    treeNode_t* result = DIV(SUB(MUL(dLeft, rightCopy), MUL(dRight, leftCopy)), MUL(rightCopy, rightCopy));
    TREE_DUMP(result, "new operation", DSL_SUCCESS);

    return result;
}

treeNode_t* differentiate(treeNode_t* node) {
    TREE_DUMP(node, "Differentiating tree", DSL_SUCCESS);
    switch (getNodeType(node)) {
        case NUMBER_TYPE: {
            setData(node, {0});
            return node;
        }
        case OPERATION_TYPE: {
            if ((int) getOperation(node) < 0 || (int) getOperation(node) >= DSL_OPERATIONS_COUNT) {
                PRINTERR("invalid operation");
                return NULL;
            }
            treeNode_t* copy = copyTree(node);
            treeNode_t* result =  DSL_OPERATIONS_INFO[getOperation(node)].diffFunction(node);

            logTex("\\begin{math}\n");
            logTex("\\frac{d}{dx}(");
            texLogRec(copy);
            logTex(") = ");
            texLogRec(result);
            logTex("\n\\end{math}\n\n");

            destroyTree(copy);
            return result;
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
            printf("%d", getNumber(node));
            return;
        }
        case OPERATION_TYPE: {
            printf("(");
            printTree(getLeft(node));

            printf(" %c ", DSL_OPERATIONS_INFO[getOperation(node)].representation);

            printTree(getRight(node));
            printf(")");
        }
        case PARAM_TYPE: {
            printf("%c", getParameter(node));
            return;
        }
        default: {
            PRINTERR("invalid node type");
        }
    }
}

double constantsFolding(treeNode_t* node, bool* changed) {
    if (node == NULL) {
        return NAN;
    }

    switch (getNodeType(node)) {
        case NUMBER_TYPE: {
            return getNumber(node);
        }
        case PARAM_TYPE: {
            return NAN;
        }
        case OPERATION_TYPE: {
            double leftValue = constantsFolding(getLeft(node), changed);
            double rightValue = constantsFolding(getRight(node), changed);
            if (isnan(leftValue) || isnan(rightValue)) {
                return NAN;
            }

            destroyTree(getLeft(node));
            setLeft(node, NULL);
            destroyTree(getRight(node));
            setRight(node, NULL);

            setNodeType(node, NUMBER_TYPE);

            double result = NAN;
            switch (getOperation(node)) {
                case NODE_ADD: {
                    result = leftValue + rightValue;
                    break;
                }
                case NODE_SUB: {
                    result = leftValue - rightValue;
                    break;
                }
                case NODE_MUL: {
                    result = leftValue * rightValue;
                    break;
                }
                case NODE_DIV: {
                    result = leftValue / rightValue;
                    break;
                }
                default: {
                    PRINTERR("invalid operation");
                    return NAN;
                }
            }

            setData(node, {(int) result});
            *changed = true;
            return result;
        }
    }
    return NAN;
}

void optimiseAddition(treeNode_t *node, bool *changed, treeNode_t *left, treeNode_t *right) {
    if (getNodeType(left) == NUMBER_TYPE && getNumber(left) == 0) {
        setData(node, getData(right));
        setNodeType(node, getNodeType(right));
        setLeft(node, getLeft(right));
        setRight(node, getRight(right));

        free(right);
        destroyTree(left);

        TREE_DUMP(node, "Optimized addition: ", DSL_SUCCESS);
        *changed = true;
    }
}

void optimizeMultiplication(treeNode_t *node, treeNode_t *left, treeNode_t *right, bool* changed) {
    if (getNodeType(left) == NUMBER_TYPE) {
        if (getNumber(left) == 0) {
            setNodeType(node, NUMBER_TYPE);
            setLeft(node, NULL);
            setRight(node, NULL);
            setData(node, {0});

            destroyTree(left);
            destroyTree(right);

            TREE_DUMP(node, "Optimized multiplication: ", DSL_SUCCESS);
            *changed = true;
        }
        else if (getNumber(left) == 1) {
            setNodeType(node, getNodeType(right));
            setLeft(node, getLeft(right));
            setRight(node, getRight(right));
            setData(node, getData(right));

            destroyTree(left);
            free(right);

            TREE_DUMP(node, "Optimized multiplication: ", DSL_SUCCESS);
            *changed = true;
        }
    }
}

void optimizeDivision(treeNode_t *node, treeNode_t *left, treeNode_t *right, bool *changed) {
    if (getNodeType(left) == NUMBER_TYPE && getNumber(left) == 0) {
        destroyTree(left);
        destroyTree(right);

        setNodeType(node, NUMBER_TYPE);
        setLeft(node, NULL);
        setRight(node, NULL);
        setData(node, {0});

        TREE_DUMP(node, "Optimized division: ", DSL_SUCCESS);
        *changed = true;
    }
    else if (getNodeType(right) == NUMBER_TYPE && getNumber(right) == 1) {
        setNodeType(node, getNodeType(left));
        setLeft(node, getLeft(left));
        setRight(node, getRight(left));
        setData(node, getData(left));

        destroyTree(right);
        free(left);

        TREE_DUMP(node, "Optimized division: ", DSL_SUCCESS);
        *changed = true;
    }
}

int removeRedurantOperations(treeNode_t* node, bool* changed) {
    if (node == NULL) {
        return DSL_SUCCESS;
    }

    TREE_DUMP(node, "Optimizing tree", DSL_SUCCESS);

    treeNode_t* left = getLeft(node);
    treeNode_t* right = getRight(node);
    switch (getNodeType(node)) {
        case NUMBER_TYPE: {
            return DSL_SUCCESS;
        }
        case PARAM_TYPE: {
            return DSL_SUCCESS;
        }
        case OPERATION_TYPE: {

            nodeOperation_t operation = getOperation(node);
            if (operation == NODE_ADD || operation == NODE_SUB) {
                optimiseAddition(node, changed, left, right);
                optimiseAddition(node, changed, right, left);
            }
            else if (operation == NODE_DIV) {
                optimizeDivision(node, left, right, changed);
            }
            else if (operation == NODE_MUL) {
                optimizeMultiplication(node, left, right, changed);
                optimizeMultiplication(node, right, left, changed);
            }
            break;
        }
    }
    removeRedurantOperations(getLeft(node),  changed);
    removeRedurantOperations(getRight(node), changed);
    TREE_DUMP(node, "After optimization: ", DSL_SUCCESS);

    return DSL_SUCCESS;
}
