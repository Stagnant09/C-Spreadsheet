#include <stdio.h>
#include "Cell.h"
#include "Functions.h"

int main(void) {
    Matrix matrix;
    init(&matrix, 6, 6);
    cell_create(&matrix, 0, 0, (CellContent){5}, 1);
    cell_create(&matrix, 1, 0, (CellContent){5}, 1);
    cell_create(&matrix, 2, 0, (CellContent){2}, 1);
    cell_create(&matrix, 3, 0, (CellContent){0}, 1);

    add(&matrix.cells[0][0], &matrix.cells[0][1], &matrix.cells[0][3]);
    print(&matrix.cells[0][3]);
    printf("\n");

    add_row(matrix.cells, &matrix.cells[0][0], &matrix.cells[0][2], &matrix.cells[0][3]);
    print(&matrix.cells[0][3]);
    printf("\n");

    print_matrix(&matrix);

    return 0;
}