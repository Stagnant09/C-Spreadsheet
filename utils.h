#ifndef TXTBSDSP_UTILS_H
#define TXTBSDSP_UTILS_H

#include <stdbool.h>

bool contains(char* cArray, int size, char c);
char* containsAny(char* cArray, int cArraySize, char* chars, int charsSize);

#endif //TXTBSDSP_UTILS_H