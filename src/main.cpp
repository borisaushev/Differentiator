
#include "dslInput.h"
#include "dslOperations.h"
#include "treeDump.h"


int main() {
    FILE* file = fopen(DSL_FILE_PATH, "r");
    if (file == NULL) {
        PRINTERR( "unable to open file");
    }
    int bytesRead = -1;
    char* buffer = NULL;
    SAFE_CALL(readFile(DSL_FILE_PATH, &buffer, &bytesRead));

    treeNode_t* root = {};
    char* cur = buffer;
    SAFE_CALL(parseNode(&cur, &root, buffer));
    TREE_DUMP(root, "parsed tree dump", DSL_SUCCESS);

    printf("result: %d\n", executeTree(root));

    SAFE_CALL(saveDslData(root));

    free(buffer);

    fclose(file);

    printf("DONE\n");
    return 0;
}
