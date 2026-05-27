#ifndef TXTBSDSP_FUNCTIONS_H
#define TXTBSDSP_FUNCTIONS_H
#include "Cell.h"

// Initialization and modification
int cell_create(Matrix* matrix, const int x, const int y, const CellContent content, const short type);
int cell_destroy(Matrix* matrix, Cell* cell);
int cell_set_content(Cell* cell, const CellContent content);
int cell_get_content(const Cell* cell, CellContent* content);
int set_matrix(Matrix* matrix, Cell** cells);
int get_matrix(const Matrix* matrix, Cell** cells);
int set_rows(Matrix* matrix, const int rows);
int get_rows(const Matrix* matrix, int* rows);
int set_cols(Matrix* matrix, const int cols);
int get_cols(const Matrix* matrix, int* cols);
int init(Matrix* matrix, const int rows, const int cols);
int matrix_destroy(Matrix* matrix);

// Type operations
int cell_set_type(Cell* cell, const short type);
int cell_get_type(const Cell* cell, short* type);

// Mathematical operations
int add(const Cell* cell1, const Cell* cell2, Cell* result);
int sub(Cell* cell1, Cell* cell2, Cell* result);
int mul(Cell* cell1, Cell* cell2, Cell* result);
int divc(Cell* cell1, Cell* cell2, Cell* result);
int mod(Cell* cell1, Cell* cell2, Cell* result);
int powc(Cell* cell1, Cell* cell2, Cell* result);

// I/O
int print(const Cell* cell);
int print_matrix(const Matrix* matrix);

// Aggregate operations
int add_row(Cell** cells, const Cell* cell1, const Cell* cell2, Cell* result);
int add_col(Cell** cells, const Cell* cell1, const Cell* cell2, Cell* result);
int avg_row(Cell** cells, const Cell* cell1, const Cell* cell2, Cell* result);
int avg_col(Cell** cells, const Cell* cell1, const Cell* cell2, Cell* result);

// Helper functions
void expand(Matrix* matrix, int rows, int cols);

#endif //TXTBSDSP_FUNCTIONS_H