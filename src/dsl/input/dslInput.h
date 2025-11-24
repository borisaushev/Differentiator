#ifndef AKINATOR_AKINATORINPUT_H
#define AKINATOR_AKINATORINPUT_H
#include "treeSctruct.h"


long getFileSize(const char* filename);

void skipSpaces(char** curPos);

int readFile(const char *file_path, char** text, int* bytes_read);

int parseNode(char** curPos, treeNode_t** cur, const char* bufferCopy);

void readUserAnswer(char inp[MAX_LINE_LENGTH]);

void writeNodeRec(treeNode_t* node, FILE* file);

int saveDslData(treeNode_t* root);

#endif //AKINATOR_AKINATORINPUT_H