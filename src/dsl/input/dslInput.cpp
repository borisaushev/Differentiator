#include "dslInput.h"

#include "common.h"
#include "treeDump.h"


long getFileSize(const char* filename) {
    struct stat st = {};
    if (stat(filename, &st) == 0) {
        return st.st_size;
    }

    return -1;
}

void skipSpaces(char** curPos) {
    while (isspace(**curPos)) {
        (*curPos)++;
    }
}

int readFile(const char *file_path, char** text, int* bytes_read) {
    int stream = open(file_path, 0);

    long file_size = getFileSize(file_path);
    DPRINTF("file size: %lu\n", file_size);

    if (file_size < 0) {
        PRINTERR("Could not open file %s\n", file_path);
        RETURN_ERR(DSL_FILE_NOT_FOUND, "Could not open file");
    }
    *text = (char *) calloc((size_t) file_size, sizeof(char));
    *bytes_read = read(stream, *text, (unsigned int) file_size);

    if (*bytes_read == -1) {
        PRINTERR("Could not read file %s with err: %s\n", file_path, strerror(errno));
        RETURN_ERR(DSL_FILE_NOT_FOUND, "Could not read file");
    }
    DPRINTF("Read %d bytes\n", *bytes_read);
    close(stream);

    *text = (char *) realloc(*text,  (size_t) *bytes_read + 2);
    (*text)[*bytes_read] = (*text)[*bytes_read-1] == '\n' ? '\0' : '\n';
    (*text)[*bytes_read] = (*text)[*bytes_read-1] == '\n' ? '\0' : '\n';
    (*text)[*bytes_read + 1] = '\0';

    return 0;
}


void dumpBuffer(char **curPos, const char *buffer) {
    char dumpString[MAX_LINE_LENGTH] = {};
    strcat(dumpString, "buffer:\n\t</p>\n\t<font color=\"#00bfff\">\n\t\t");
    strncat(dumpString, buffer,  strlen(buffer) - strlen(*curPos));
    strcat(dumpString, "\n\t</font>\n");

    strcat(dumpString, "\t<font color=\"#0e2466\">\n\t\t");
    strcat(dumpString, *curPos);
    strcat(dumpString, "\n\t</font>");

    treeLog(dumpString);
}

int parseNode(char** curPos, treeNode_t** cur, const char* buffer) {
    skipSpaces(curPos);

    treeLog("Parsing node");
    dumpBuffer(curPos, buffer);

    if (**curPos == '(') {
        treeLog("Read '(' ");
        skipSpaces(curPos);
        (*curPos)++;

        treeLog("skipped '('");
        dumpBuffer(curPos, buffer);
        skipSpaces(curPos);

        switch(**curPos) {
            case '+': {
                createOperation(cur, NODE_ADD, OPERATION_TYPE, NULL, NULL);
                break;
            }
            case '-': {
                createOperation(cur, NODE_SUB, OPERATION_TYPE, NULL, NULL);
                break;
            }
            case '*': {
                createOperation(cur, NODE_MUL, OPERATION_TYPE, NULL, NULL);
                break;
            }
            case '/': {
                createOperation(cur, NODE_DIV, OPERATION_TYPE, NULL, NULL);
                break;
            }
            case 'x': {
                createParameter(cur, 'x', PARAM_TYPE, NULL, NULL);
                break;
            }
            default: {
                if (**curPos >= '0' && **curPos <= '9') {
                    int input = -1, n = -1;
                    if (sscanf(*curPos, "%d%n", &input, &n) != 1) {
                        PRINTERR("Expected number, Invalid character '%c' (%d) at %s:%d:%zu\n",
                             **curPos, **curPos, DSL_FILE_PATH, 1, (*curPos - buffer + 1));
                    }
                    *curPos += n;
                    createValue(cur, input, NUMBER_TYPE, NULL, NULL);
                }
                else {
                    PRINTERR("Expected number, parameter or operation, Invalid character '%c' (%d) at %s:%d:%zu\n",
                             **curPos, **curPos, DSL_FILE_PATH, 1, (*curPos - buffer + 1));
                    RETURN_ERR(DSL_INVALID_INPUT, "Expected number, parameter or operation");
                }
            }
        }

        treeLog("Created new node");
        TREE_DUMP(*cur, "Created node", 0);
        (*curPos)++;


        skipSpaces(curPos);
        treeLog("skipped data");
        dumpBuffer(curPos, buffer);

        treeLog("Parsing left subtree");
        SAFE_CALL(parseNode(curPos, &(*cur)->left, buffer));
        treeLog("Parsed left subtree");
        TREE_DUMP(*cur, "Parsed left subtree", DSL_SUCCESS);

        treeLog("Parsing right subtree");
        SAFE_CALL(parseNode(curPos, &(*cur)->right, buffer));
        treeLog("Parsed right subtree");
        TREE_DUMP(*cur, "Parsed right subtree", DSL_SUCCESS);

        skipSpaces(curPos);
        if (**curPos != ')') {
            PRINTERR("Expecting ')', Invalid character '%c' (%d) at %s:%d:%zu\n", **curPos, **curPos, DSL_FILE_PATH, 1, (*curPos - buffer + 1));

            RETURN_ERR(DSL_INVALID_INPUT, "expected ')'");
        }
        (*curPos)++;
        skipSpaces(curPos);
        treeLog("skipped ')'");
        dumpBuffer(curPos, buffer);

        return DSL_SUCCESS;
    }
    else if (strncmp(*curPos, NULL_NODE_STRING, 3) == 0) {
            treeLog("found nil node");
            *curPos += strlen(NULL_NODE_STRING);

            treeLog("Skipped nil");
            dumpBuffer(curPos, buffer);

            return DSL_SUCCESS;
        }
    else {
        PRINTERR("Expected '(' or nil, invalid character '%c', (%d) at %s:%d:%zu\n", **curPos, **curPos, DSL_FILE_PATH, 1, (*curPos - buffer + 1));
        RETURN_ERR(DSL_INVALID_INPUT, "invalid input tree");
    }
}

void readUserAnswer(char inp[MAX_LINE_LENGTH]) {
    scanf("%[^\n]", inp);
    getchar();
}

void writeNodeRec(treeNode_t* node, FILE* file) {
    if (node == NULL) {
        fprintf(file, NULL_NODE_STRING);
        fprintf(file, " ");
    }
    else {
        fprintf(file, "(");
        switch(getNodeType(node)) {
            case NUMBER_TYPE: {
                fprintf(file, " %d ", getData(node).number);
                break;
            }
            case OPERATION_TYPE: {
                switch (getData(node).operation) {
                    case NODE_ADD: {
                        fprintf(file, " + ");
                        break;
                    }
                    case NODE_SUB: {
                        fprintf(file, " - ");
                        break;
                    }
                    case NODE_MUL: {
                        fprintf(file, "*");
                        break;
                    }
                    case NODE_DIV: {
                        fprintf(file, "/");
                        break;
                    }
                    default: {
                        PRINTERR("Invalid node operation");
                    }
                }
                break;
            }
            case PARAM_TYPE: {
                fprintf(file, " %c ", getData(node).parameter);
                break;
            }
            default: {
                PRINTERR("UNEXPECTED NODE TYPE: %d\n", getNodeType(node));
                fprintf(file, "UNEXPECTED NODE TYPE");
            }
        }
        writeNodeRec(getLeft(node), file);
        writeNodeRec(getRight(node), file);
        fprintf(file, ") ");
    }
}

int saveDslData(treeNode_t* root) {
    FILE* file = fopen(DSL_FILE_PATH, "w");
    if (file == NULL) {
        RETURN_ERR(DSL_NULL_PTR, "unable to open file");
    }
    writeNodeRec(root, file);

    fclose(file);
    return DSL_SUCCESS;
}