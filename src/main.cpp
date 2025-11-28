
#include "dslInput.h"
#include "dslOperations.h"
#include "treeDump.h"


int main() {
    FILE* file = fopen(DSL_FILE_PATH, "r");
    if (file == NULL) {
        PRINTERR("unable to open file");
    }
    int bytesRead = -1;
    char* buffer = NULL;
    SAFE_CALL(readFile(DSL_FILE_PATH, &buffer, &bytesRead));

    initDslParametersValues();

    char* cur = buffer;
    treeNode_t* root = getExpression(&cur, buffer);
    if (*cur != '\0') {
        PRINTERR("Invalid input. Invalid character '%c' (%d) at %s:%d:%zu\n",
                 *cur, *cur, DSL_FILE_PATH, 1, (cur - buffer + 1));
        return DSL_INVALID_INPUT;
    }
    // SAFE_CALL(parseNode(&cur, &root, buffer));
    TREE_DUMP(root, "parsed tree dump", DSL_SUCCESS);

    printTree(root);
    logTex("\\section{parsed expression}\n");
    logTreeTex(root);
    logTex("\ngiven that:\n");
    for (size_t i = 0; i < getParametersCount(); i++) {
        dslParameter_t* param = getParameter(i);
        logTex("%s = %d\n", param->name, param->value);
    }

    //префикс функция и кмп
    bool changed = true;
    while (changed) {
        changed = false;
        constantsFolding (root, &changed);
        removeRedurantOperations(root, &changed);
        TREE_DUMP(root, "After two optimizations in main\n", DSL_SUCCESS);
    }

    printf("\nsimplified\n");
    printTree(root);
    TREE_DUMP(root, "simplified tree", DSL_SUCCESS);
    logTex("\n\\section{simplified expression}\n");
    logTreeTex(root);

    printf(" = %.2f\n", findTreeValue(root));

    logTex("\\section{expression derivative}\n");
    printf("d/dx(");
    printTree(root);
    printf(") = ");

    root = differentiate(root);
    TREE_DUMP(root, "derivative", DSL_SUCCESS);
    logTex("Final derivative:\n");
    logTreeTex(root);
    printTree(root);

    changed = true;
    while (changed) {
        changed = false;
        constantsFolding(root, &changed);
        removeRedurantOperations(root, &changed);
        TREE_DUMP(root, "After two optimizations in main\n", DSL_SUCCESS);
    }

    TREE_DUMP(root, "simplified tree", DSL_SUCCESS);
    logTex("\n\\section{simplified derivative}\n");
    logTreeTex(root);

    printf("\nsimplified:\n");
    printTree(root);
    printf(" = %.2f\n", findTreeValue(root));

    closeTex();
    free(buffer);
    fclose(file);

    printf("DONE\n");
    return 0;
}
