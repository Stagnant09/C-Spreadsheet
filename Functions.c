#include <stdio.h>
#include <stdlib.h>
#include "Cell.h"
#include "Functions.h"

int cell_create(Matrix* matrix, Cell* cell, const int x, const int y, const CellContent content, const short type) {
    if (x < 0 || y < 0) {
        return -1;
    }
    if (x >= matrix->rows || y >= matrix->cols) {
        expand(matrix, x + 1, y + 1);
    }
    cell->x = x;
    cell->y = y;
    cell->content = content;
    cell->type = type;
    matrix->cells[x][y] = *cell;
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
    } else {
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
}

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
        add(&cells[j][row], result, result);
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
        add(&cells[col][i], result, result);
    }
    return 0;
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