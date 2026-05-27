#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Cell.h"
#include "Functions.h"

int cell_create(Matrix* matrix, const int x, const int y, const CellContent content, const short type) {
    if (x < 0 || y < 0) {
        return -1;
    }
    if (x >= matrix->cols || y >= matrix->rows) {
        expand(matrix, y + 1, x + 1);
    }
    Cell* cell = &matrix->cells[y][x];
    cell->x = x;
    cell->y = y;
    cell->content = content;
    cell->type = type;
    return 0;
}

int cell_destroy(Matrix* matrix, Cell* cell) {
    cell->x = 0;
    cell->y = 0;
    cell->content.value = 0;
    cell->type = 0;
    return 0;
}

int cell_set_content(Cell* cell, const CellContent content) {
    cell->content = content;
    return 0;
}

int cell_get_content(const Cell* cell, CellContent* content) {
    *content = cell->content;
    return 0;
}

int cell_set_type(Cell* cell, const short type) {
    cell->type = type;
    return 0;
}

int cell_get_type(const Cell* cell, short* type) {
    *type = cell->type;
    return 0;
}

int set_matrix(Matrix* matrix, Cell** cells) {
    matrix->cells = cells;
    return 0;
}

int get_matrix(const Matrix* matrix, Cell** cells) {
    *cells = *matrix->cells;
    return 0;
}

int set_rows(Matrix* matrix, const int rows) {
    matrix->rows = rows;
    return 0;
}

int get_rows(const Matrix* matrix, int* rows) {
    *rows = matrix->rows;
    return 0;
}

int set_cols(Matrix* matrix, const int cols) {
    matrix->cols = cols;
    return 0;
}

int get_cols(const Matrix* matrix, int* cols) {
    *cols = matrix->cols;
    return 0;
}

int init(Matrix *matrix, const int rows, const int cols) {
    matrix->rows = rows;
    matrix->cols = cols;
    matrix->cells = (Cell**)malloc(rows * sizeof(Cell *));
    for (int i = 0; i < rows; i++) {
        matrix->cells[i] = (Cell*)malloc(cols * sizeof(Cell));
        for (int j = 0; j < cols; j++) {
            matrix->cells[i][j] = (Cell){0, 0, {0}, 0};
        }
    }
    return 0;
}

int add(const Cell* cell1, const Cell* cell2, Cell* result) {
    if (cell1->type != cell2->type) {
        return -1; // Types do not match
    }
    CellContent cc;
    switch (cell1->type) {
        case 0:
            return -1; // No type set
            break;
        case 1:
            cc.value = cell1->content.value + cell2->content.value;
            result->content.value = cc.value;
            break;
        case 2:
            cc.fvalue = cell2->content.fvalue + cell1->content.fvalue;
            result->content.fvalue = cc.fvalue;
            break;
        case 3:
            cc.dvalue = cell1->content.dvalue + cell2->content.dvalue;
            result->content.dvalue = cc.dvalue;
            break;
        case 4:
            return -1; // Not implemented
            break;
        default:
            return -1;
    }
    return 0;
}

int sub(const Cell* cell1, const Cell* cell2, Cell* result) {
    if (cell1->type != cell2->type) {
        return -1;
    }
    CellContent cc;
    switch (cell1->type) {
        case 0:
            return -1; // No type set
            break;
        case 1:
            cc.value = cell1->content.value - cell2->content.value;
            result->content.value = cc.value;
            break;
        case 2:
            cc.fvalue = cell1->content.fvalue - cell2->content.fvalue;
            result->content.fvalue = cc.fvalue;
            break;
        case 3:
            cc.dvalue = cell1->content.dvalue - cell2->content.dvalue;
            result->content.dvalue = cc.dvalue;
            break;
        case 4:
            return -1; // Not implemented
            break;
        default:
            return -1;
    }
    return 0;
}

int mul(const Cell* cell1, const Cell* cell2, Cell* result) {
    if (cell1->type != cell2->type) {
        return -1;
    }
    CellContent cc;
    switch (cell1->type) {
        case 0:
            return -1;
            break;
        case 1:
            cc.value = cell1->content.value * cell2->content.value;
            result->content.value = cc.value;
            break;
        case 2:
            cc.fvalue = cell1->content.fvalue * cell2->content.fvalue;
            result->content.fvalue = cc.fvalue;
            break;
        case 3:
            cc.dvalue = cell1->content.dvalue * cell2->content.dvalue;
            result->content.dvalue = cc.dvalue;
            break;
        case 4:
            return -1;
            break;
        default:
            return -1;
    }
    return 0;
}

int divc(const Cell* cell1, const Cell* cell2, Cell* result) {
    if (cell1->type != cell2->type) {
        return -1;
    }
    CellContent cc;
    switch (cell1->type) {
        case 0:
            return -1;
            break;
        case 1:
            if (cell2->content.value == 0) return -1;
            cc.dvalue = (double)cell1->content.value / (double)cell2->content.value;
            result->content.dvalue = cc.dvalue;
            result->type = 3;
            break;
        case 2:
            if (cell2->content.fvalue == 0.0f) return -1;
            cc.fvalue = cell1->content.fvalue / cell2->content.fvalue;
            result->content.fvalue = cc.fvalue;
            break;
        case 3:
            if (cell2->content.dvalue == 0.0) return -1;
            cc.dvalue = cell1->content.dvalue / cell2->content.dvalue;
            result->content.dvalue = cc.dvalue;
            break;
        case 4:
            return -1;
            break;
        default:
            return -1;
    }
    return 0;
}

int mod(const Cell* cell1, const Cell* cell2, Cell* result) {
    if (cell1->type != cell2->type) {
        return -1;
    }
    CellContent cc;
    switch (cell1->type) {
        case 1:
            if (cell2->content.value == 0) return -1;
            cc.value = cell1->content.value % cell2->content.value;
            result->content.value = cc.value;
            break;
        default:
            return -1; // Only integers for modulo
    }
    return 0;
}


int powc(const Cell* cell1, const Cell* cell2, Cell* result) {
    if (cell1->type != cell2->type) {
        return -1;
    }
    CellContent cc;
    switch (cell1->type) {
        case 1:
            cc.value = (int)pow(cell1->content.value, cell2->content.value);
            result->content.value = cc.value;
            break;
        case 2:
            cc.fvalue = powf(cell1->content.fvalue, cell2->content.fvalue);
            result->content.fvalue = cc.fvalue;
            break;
        case 3:
            cc.dvalue = pow(cell1->content.dvalue, cell2->content.dvalue);
            result->content.dvalue = cc.dvalue;
            break;
        default:
            return -1;
    }
    return 0;
}

/**
 * Print a cell in a "(x, y) value" format
 * @param cell The cell to print
 * @return 0 on success, -1 on failure
 */
int print(const Cell* cell) {
    switch (cell->type) {
        case 0:
            return -1;
        case 1:
            printf("(%d, %d) %d", cell->x, cell->y, cell->content.value);
            break;
        case 2:
            printf("(%d, %d) %f", cell->x, cell->y, cell->content.fvalue);
            break;
        case 3:
            printf("(%d, %d) %f", cell->x, cell->y, cell->content.dvalue);
            break;
        case 4:
            printf("(%d, %d) %s", cell->x, cell->y, cell->content.string);
            break;
        default:
            return -1;
    }
    return 0;
}

int add_row(Cell** cells, const Cell* cell1, const Cell* cell2, Cell* result) {
    cell_set_type(result, cell1->type);
    cell_set_content(result, (CellContent){0});
    if (cell1->y != cell2->y) {
        return -1; // Not the same row
    }
    const int row = cell1->y;
    for (int j = cell1->x; j <= cell2->x; j++) {
        add(&cells[row][j], result, result);
    }
    return 0;
}

int add_col(Cell** cells, const Cell* cell1, const Cell* cell2, Cell* result) {
    cell_set_type(result, cell2->type);
    cell_set_content(result, (CellContent){0});
    if (cell1->x != cell2->x) {
        return -1; // Not the same column
    }
    const int col = cell1->x;
    for (int i = cell1->y; i <= cell2->y; i++) {
        add(&cells[i][col], result, result);
    }
    return 0;
}

int avg_row(Cell** cells, const Cell* cell1, const Cell* cell2, Cell* result) {
    if (add_row(cells, cell1, cell2, result) != 0) return -1;
    int count = cell2->x - cell1->x + 1;
    if (count <= 0) return -1;
    Cell divisor = *result;
    switch (result->type) {
        case 1: divisor.content.value = count; break;
        case 2: divisor.content.fvalue = (float)count; break;
        case 3: divisor.content.dvalue = (double)count; break;
        default: return -1;
    }
    return divc(result, &divisor, result);
}

int avg_col(Cell** cells, const Cell* cell1, const Cell* cell2, Cell* result) {
    if (add_col(cells, cell1, cell2, result) != 0) return -1;
    int count = cell2->y - cell1->y + 1;
    if (count <= 0) return -1;
    Cell divisor = *result;
    switch (result->type) {
        case 1: divisor.content.value = count; break;
        case 2: divisor.content.fvalue = (float)count; break;
        case 3: divisor.content.dvalue = (double)count; break;
        default: return -1;
    }
    return divc(result, &divisor, result);
}

void expand(Matrix* matrix, int rows, int cols) {
    int new_rows = (rows > matrix->rows) ? rows : matrix->rows;
    int new_cols = (cols > matrix->cols) ? cols : matrix->cols;

    Cell** new_cells = (Cell**)malloc(new_rows * sizeof(Cell*));
    for (int i = 0; i < new_rows; i++) {
        new_cells[i] = (Cell*)malloc(new_cols * sizeof(Cell));
        for (int j = 0; j < new_cols; j++) {
            if (i < matrix->rows && j < matrix->cols) {
                new_cells[i][j] = matrix->cells[i][j];
            } else {
                new_cells[i][j] = (Cell){0, 0, {0}, 0};
            }
        }
    }

    for (int i = 0; i < matrix->rows; i++) {
        free(matrix->cells[i]);
    }
    free(matrix->cells);

    matrix->cells = new_cells;
    matrix->rows = new_rows;
    matrix->cols = new_cols;
}

int print_matrix(const Matrix* matrix) {
    if (matrix == NULL || matrix->cells == NULL) {
        return -1;
    }

    // Print top border
    printf(" ");
    for (int j = 0; j < matrix->cols; j++) {
        printf("------------ ");
    }
    printf("\n");

    for (int i = 0; i < matrix->rows; i++) {
        // Print cell content row
        printf("|");
        for (int j = 0; j < matrix->cols; j++) {
            Cell* cell = &matrix->cells[i][j];
            char buffer[12];
            buffer[0] = '\0';

            switch (cell->type) {
                case 1:
                    snprintf(buffer, sizeof(buffer), "%d", cell->content.value);
                    break;
                case 2:
                    snprintf(buffer, sizeof(buffer), "%.2f", cell->content.fvalue);
                    break;
                case 3:
                    snprintf(buffer, sizeof(buffer), "%.2lf", cell->content.dvalue);
                    break;
                case 4:
                    if (cell->content.string) {
                        snprintf(buffer, sizeof(buffer), "%s", cell->content.string);
                    }
                    break;
                default:
                    // Empty cell
                    break;
            }
            printf(" %-10s |", buffer);
        }
        printf("\n");

        // Print bottom border for each row
        printf("|");
        for (int j = 0; j < matrix->cols; j++) {
            printf("____________|");
        }
        printf("\n");
    }

    return 0;
}