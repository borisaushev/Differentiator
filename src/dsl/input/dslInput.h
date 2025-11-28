#ifndef AKINATOR_AKINATORINPUT_H
#define AKINATOR_AKINATORINPUT_H
#include "treeSctruct.h"


treeNode_t* getHighPriorityExpression(char** curPos, char* start);
treeNode_t* getExpression(char** curPos, char* start);
treeNode_t* getNumber(char** curPos, char* start);

long getFileSize(const char* filename);

void skipSpaces(char** curPos);
void dumpBuffer(char **curPos, const char *buffer);

int readFile(const char *file_path, char** text, int* bytes_read);

void writeNodeRec(treeNode_t* node, FILE* file);

int saveDslData(treeNode_t* root);

#endif //AKINATOR_AKINATORINPUT_H