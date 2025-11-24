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
    differentiate(getLeft(node));
    TREE_DUMP(node, "after differentiating left tree", DSL_SUCCESS);

    treeLog("Differentiating left tree");
    differentiate(getRight(node));
    TREE_DUMP(node, "after differentiating right tree", DSL_SUCCESS);

    return node;
}

treeNode_t* diffSubtraction(treeNode_t* node) {
    treeLog("Differentiating subtraction");
    differentiate(getLeft(node));
    TREE_DUMP(node, "after differentiating left tree", DSL_SUCCESS);

    treeLog("Differentiating left tree");
    differentiate(getRight(node));
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
            return DSL_OPERATIONS_INFO[getOperation(node)].diffFunction(node);
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

double constantsFolding(treeNode_t* node) {
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
            double leftValue = constantsFolding(getLeft(node));
            double rightValue = constantsFolding(getRight(node));
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