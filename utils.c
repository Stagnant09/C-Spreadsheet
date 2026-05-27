#include <stdbool.h>
#include <string.h>

bool contains(char *cArray, int size, char c) {
    for (int i = 0; i < size; i++) {
        if (cArray[i] == c) return true;
    }
    return false;
}

char* containsAny(char *cArray, int cArraySize, char* chars, int charsSize) {
    for (int i = 0; i < cArraySize; i++) {
        if (contains(chars, charsSize, cArray[i])) return &cArray[i];
    }
    return NULL;
}
