#include <stdio.h>
#include "Cell.h"
#include "Functions.h"

int main(void) {
    Matrix matrix;
    init(&matrix, 10, 10);
    Cell c1;
    cell_create(&matrix, &c1, 0, 0, (CellContent){5}, 1);
    Cell c2;
    cell_create(&matrix, &c2, 1, 0, (CellContent){5}, 1);
    Cell c3;
    cell_create(&matrix, &c3, 2, 0, (CellContent){2}, 1);
    Cell result;
    cell_create(&matrix, &result, 3, 0, (CellContent){0}, 1);
    add(&c1, &c2, &result);
    print(&result);
    printf("\n");
    add_row(matrix.cells, &c1, &c3, &result);
    print(&result);
    cell_destroy(&matrix, &c1);
    cell_destroy(&matrix, &c2);
    cell_destroy(&matrix, &c3);
    cell_destroy(&matrix, &result);
    return 0;
}