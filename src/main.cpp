
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

    treeNode_t* root = {};
    char* cur = buffer;
    SAFE_CALL(parseNode(&cur, &root, buffer));
    TREE_DUMP(root, "parsed tree dump", DSL_SUCCESS);

    printTree(root);
    logTex("\\section{parsed expression}\n");
    logTreeTex(root);
    logTex("\ngiven that x = %d\n", getParameterValue('x'));

    constantsFolding(root);
    printf("\nsimplified\n");
    printTree(root);
    TREE_DUMP(root, "simplified tree", DSL_SUCCESS);
    logTex("\n\\section{simplified expression}\n");
    logTreeTex(root);

    printf(" = %d\n", findTreeValue(root));

    printf("d/dx(");
    printTree(root);
    printf(") = ");

    root = differentiate(root);
    TREE_DUMP(root, "derivative", DSL_SUCCESS);
    logTex("\\section{expression derivative}\n");
    logTreeTex(root);

    constantsFolding(root);
    printTree(root);
    TREE_DUMP(root, "simplified tree", DSL_SUCCESS);
    logTex("\n\\section{simplified expression}\n");
    logTreeTex(root);

    printTree(root);
    printf(" = ");
    printf("%d, when x = %d\n", findTreeValue(root), getParameterValue('x'));

    closeTex();
    free(buffer);
    fclose(file);

    printf("DONE\n");
    return 0;
}
