#ifndef TXTBSDSP_CELL_H
#define TXTBSDSP_CELL_H

typedef union {
    int value;
    float fvalue;
    double dvalue;
    char* string;
} CellContent ;

typedef struct {
    int x;
    int y;
    CellContent content;
    short type;
} Cell ;

typedef struct {
    Cell** cells;
    int rows;
    int cols;
} Matrix;

#endif //TXTBSDSP_CELL_H