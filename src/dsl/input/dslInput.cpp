#include "dslInput.h"

#include "common.h"
#include "dslOperations.h"
#include "treeDump.h"

treeNode_t* getNumber(char** curPos, char* start) {
    skipSpaces(curPos);
    int val = 0;
    const char* curStart = *curPos;
    while (isdigit(**curPos)) {
        val = val * 10 + (**curPos - '0');
        *curPos += 1;
    }
    if (curStart == *curPos) {
        PRINTERR("Expected number, parameter or operation, Invalid character '%c' (%d) at %s:%d:%zu\n",
                 **curPos, **curPos, DSL_FILE_PATH, 1, (*curPos - start + 1));

        return NULL;
    }
    skipSpaces(curPos);

    dumpBuffer(curPos, start);
    return createValue(val);
}

treeNode_t* getParameter(char** curPos, char* start) {
    skipSpaces(curPos);

    char ch = **curPos;
    if (!isalpha(ch)) {
        PRINTERR("Expected parameter name. Invalid character '%c' (%d) at %s:%d:%zu\n",
                 **curPos, **curPos, DSL_FILE_PATH, 1, (*curPos - start + 1));
        return NULL;
    }
    char paramName[MAX_LINE_LENGTH] = {};
    int skipped = 0;
    sscanf(*curPos, "%[A-Za-z]%n", paramName, &skipped);
    *curPos += skipped;
    skipSpaces(curPos);

    dumpBuffer(curPos, start);
    return createParameter(paramName);
}

treeNode_t* getBlockExpression(char** curPos, char* start) {
    skipSpaces(curPos);
    char ch = **curPos;
    if (ch == '(') {
        *curPos += 1;
        skipSpaces(curPos);
        treeNode_t* expression = getExpression(curPos, start);
        if (expression == NULL) {
            return NULL;
        }
        skipSpaces(curPos);
        if (**curPos != ')') {
            PRINTERR("Expected closing bracket ')'. Invalid character '%c' (%d) at %s:%d:%zu\n",
                 **curPos, **curPos, DSL_FILE_PATH, 1, (*curPos - start + 1));
            return NULL;
        }
        *curPos += 1;
        skipSpaces(curPos);
        dumpBuffer(curPos, start);
        return expression;
    }
    else if (isdigit(ch)) {
        treeNode_t* result = getNumber(curPos, start);
        dumpBuffer(curPos, start);
        return result;
    }
    else {
        treeNode_t* result = getParameter(curPos, start);
        dumpBuffer(curPos, start);
        return result;
    }
}

treeNode_t* getExpression(char** curPos, char* start) {
    treeNode_t* result = getHighPriorityExpression(curPos, start);
    if (result == NULL) {
        return NULL;
    }
    while (**curPos == '+' || **curPos == '-') {
        char operation = **curPos;
        *curPos += 1;
        skipSpaces(curPos);

        treeNode_t* expression = getHighPriorityExpression(curPos, start);
        if (expression == NULL) {
            return NULL;
        }

        if (operation == '+') {
            result = createOperation(NODE_ADD, result, expression);
        }
        else {
            result = createOperation(NODE_SUB, result, expression);
        }
    }

    dumpBuffer(curPos, start);
    return result;
}

treeNode_t* getHighPriorityExpression(char** curPos, char* start) {
    treeNode_t* result = getBlockExpression(curPos, start);
    if (result == NULL) {
        return NULL;
    }
    while (**curPos == '*' || **curPos == '/') {
        char operation = **curPos;
        *curPos += 1;
        skipSpaces(curPos);

        treeNode_t* expression = getBlockExpression(curPos, start);
        if (expression == NULL) {
            return NULL;
        }

        if (operation == '*') {
            result = createOperation(NODE_MUL, result, expression);
        }
        else {
            result = createOperation(NODE_DIV, result, expression);
        }
    }

    dumpBuffer(curPos, start);
    return result;
}

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

