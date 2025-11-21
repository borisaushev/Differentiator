#ifndef AKINATOR_AKINATORINPUT_H
#define AKINATOR_AKINATORINPUT_H
#include "treeSctruct.h"

typedef enum dsl_error {
    DSL_SUCCESS = 0,
    DSL_NULL_PTR,
    DSL_INVALID_CAPACITY,
    DSL_INVALID_INDEX,
    DSL_INVALID_PTR,
    DSL_FILE_NOT_FOUND,
    DSL_INVALID_INPUT,
    DSL_CANT_OPEN_FILE
} dsl_error_t;

const int MAX_LINE_LENGTH = 10000;

const char* const NULL_NODE_STRING = "nil";

const char* const DSL_FILE_PATH = "../files/dsl/dsl.txt";

long getFileSize(const char* filename);

void skipSpaces(char** curPos);

int readFile(const char *file_path, char** text, int* bytes_read);

int parseNode(char** curPos, treeNode_t** cur, const char* bufferCopy);

void readUserAnswer(char inp[MAX_LINE_LENGTH]);

void writeNodeRec(treeNode_t* node, FILE* file);

int saveDslData(treeNode_t* root);

#endif //AKINATOR_AKINATORINPUT_H