#include "dslOperations.h"

#include "dslInput.h"
#include "treeDump.h"
#include "treeSctruct.h"

double addFunction(treeNode_t *node) {
    double leftValue = findTreeValue(getLeft(node));
    double rightValue = findTreeValue(getRight(node));
    treeLog("adding trees");
    double value = leftValue + rightValue;

    logTex("\\begin{math}\n");
    texLogRec(node);
    logTex(" = %.1f + %.1f = %.1f", leftValue, rightValue, value);
    logTex("\n\\end{math}\n\n");

    treeLog("Add result: %.1f", value);
    TREE_DUMP(node, "After add", DSL_SUCCESS);
    return value;
}

double subtractionFunction(treeNode_t *node) {
    double leftValue = findTreeValue(getLeft(node));
    double rightValue = findTreeValue(getRight(node));

    treeLog("subtracting trees");
    double value =  leftValue - rightValue;

    logTex("\\begin{math}\n");
    texLogRec(node);
    logTex(" = %.1f - %.1f = %.1f", leftValue, rightValue, value);
    logTex("\n\\end{math}\n\n");

    treeLog("Sub result: %.1f", value);
    TREE_DUMP(node, "After sub", DSL_SUCCESS);
    return value;
}

double multiplicationFunction(treeNode_t *node) {
    double leftValue = findTreeValue(getLeft(node));
    double rightValue = findTreeValue(getRight(node));
    treeLog("multiplying trees");
    double value =  leftValue * rightValue;

    logTex("\\begin{math}\n");
    texLogRec(node);
    logTex(" = %.1f * %.1f = %.1f", leftValue, rightValue, value);
    logTex("\n\\end{math}\n\n");

    treeLog("Mul result: %.1f", value);
    TREE_DUMP(node, "After mul", DSL_SUCCESS);
    return value;
}

double divisionFunction(treeNode_t *node) {
    double leftValue = findTreeValue(getLeft(node));
    double rightValue = findTreeValue(getRight(node));
    treeLog("dividing trees");
    double value =  leftValue / rightValue;

    logTex("\\begin{math}\n");
    texLogRec(node);
    logTex(" = \\frac{%.1f}{%.1f} = %.1f", leftValue, rightValue, value);
    logTex("\n\\end{math}\n\n");

    treeLog("Div result: %.1f", value);
    TREE_DUMP(node, "After div", DSL_SUCCESS);
    return value;
}

double sinFunction(treeNode_t *node) {
    double expressionValue = findTreeValue(getLeft(node));
    treeLog("evaluating sin");
    double result = sin(expressionValue);

    logTex("\\begin{math}\n");
    texLogRec(node);
    logTex(" = \\sin{%.1f} = %.1f", expressionValue, result);
    logTex("\n\\end{math}\n\n");

    treeLog("Sin result: %.1f", result);
    TREE_DUMP(node, "After sin", DSL_SUCCESS);
    return result;
}

double cosFunction(treeNode_t *node) {
    double expressionValue = findTreeValue(getLeft(node));
    treeLog("evaluating cos");
    double result = cos(expressionValue);

    logTex("\\begin{math}\n");
    texLogRec(node);
    logTex(" = \\cos{%.1f} = %.1f", expressionValue, result);
    logTex("\n\\end{math}\n\n");

    treeLog("cos result: %.1f", result);
    TREE_DUMP(node, "After cos", DSL_SUCCESS);
    return result;
}

double findTreeValue(treeNode_t* node) {
    if (node == NULL) {
        PRINTERR("NULL NODE");
        return 0;
    }
    treeLog("Finding tree value");
    TREE_DUMP(node, "findTreeValue", DSL_SUCCESS);
    switch (getNodeType(node)) {
        case NUMBER_TYPE: {
            treeLog("returning number: %d", getNumber(node));
            return getNumber(node);
        }
        case PARAM_TYPE: {
            treeLog("Returning parameter: %s = %d", getParameter(node)->name, getParameter(node)->value);
            return getParameter(node)->value;
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

static treeNode* copyTree(treeNode_t* node) {
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
    setOperation(node, NODE_ADD);
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
    setOperation(node, NODE_DIV);
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

treeNode_t* diffSin(treeNode_t* node) {
    treeLog("Differentiating sin");
    setOperation(node, NODE_MUL);
    treeNode_t* expression = getLeft(node);

    treeNode_t* expressionCopy = copyTree(expression);
    TREE_DUMP(expressionCopy, "copied left tree", DSL_SUCCESS);

    treeNode_t* dExpression = differentiate(expression);
    TREE_DUMP(dExpression, "d/dx Left tree", DSL_SUCCESS);

    treeNode_t* result = MUL(COS(expressionCopy), dExpression);
    TREE_DUMP(result, "new operation", DSL_SUCCESS);

    return result;
}

treeNode_t* diffCos(treeNode_t* node) {
    treeLog("Differentiating Cos");
    setOperation(node, NODE_MUL);
    treeNode_t* expression = getLeft(node);

    treeNode_t* expressionCopy = copyTree(expression);
    TREE_DUMP(expressionCopy, "copied left tree", DSL_SUCCESS);

    treeNode_t* dExpression = differentiate(expression);
    TREE_DUMP(dExpression, "d/dx Left tree", DSL_SUCCESS);

    treeNode_t* result = MUL(NEG(COS(expressionCopy)), dExpression);
    TREE_DUMP(result, "new operation", DSL_SUCCESS);

    return result;
}

treeNode_t* differentiate(treeNode_t* node) {
    TREE_DUMP(node, "Differentiating tree", DSL_SUCCESS);
    switch (getNodeType(node)) {
        case NUMBER_TYPE: {
            setNumber(node, 0);
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
            const char* paramName = getData(node).parameter->name;
            setNumber(node, !strcmp("x", paramName) ? 1 : 0);
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
            operationInfo operation = DSL_OPERATIONS_INFO[getOperation(node)];
            if (operation.isAFunction) {
                printf("%s(", operation.representation);
                printTree(getLeft(node));
                printf(")");
                return;
            }

            printf("(");
            printTree(getLeft(node));

            printf(" %s ", operation.representation);

            printTree(getRight(node));
            printf(")");
            return;
        }
        case PARAM_TYPE: {
            printf("%s", getParameter(node)->name);
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

            setNumber(node, (int) result);
            *changed = true;
            return result;
        }
        default: {
            PRINTERR("invalid node type");
            return NAN;
        }
    }
    return NAN;
}

static void optimiseAddition(treeNode_t *node, bool *changed, treeNode_t *left, treeNode_t *right) {
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

static void optimizeMultiplication(treeNode_t *node, treeNode_t *left, treeNode_t *right, bool* changed) {
    if (getNodeType(left) == NUMBER_TYPE) {
        if (getNumber(left) == 0) {
            setNodeType(node, NUMBER_TYPE);
            setLeft(node, NULL);
            setRight(node, NULL);
            setNumber(node, 0);

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

static void optimizeDivision(treeNode_t *node, treeNode_t *left, treeNode_t *right, bool *changed) {
    if (getNodeType(left) == NUMBER_TYPE && getNumber(left) == 0) {
        destroyTree(left);
        destroyTree(right);

        setNodeType(node, NUMBER_TYPE);
        setLeft(node, NULL);
        setRight(node, NULL);
        setNumber(node, 0);

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
        default: {
            PRINTERR("invalid node type");
            return DSL_INVALID_INPUT;
        }
    }
    removeRedurantOperations(getLeft(node),  changed);
    removeRedurantOperations(getRight(node), changed);
    TREE_DUMP(node, "After optimization: ", DSL_SUCCESS);

    return DSL_SUCCESS;
}
