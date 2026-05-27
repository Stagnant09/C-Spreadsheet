#include "Parser.h"
#include "Functions.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

static int parse_cell_name(const char* name, int* x, int* y) {
    if (!name || !isalpha(name[0])) return -1;
    *x = toupper(name[0]) - 'A';
    *y = atoi(name + 1);
    return 0;
}

void process_command(Matrix* matrix, char* command) {
    char* target_str = strtok(command, "=");
    char* expr_str = strtok(NULL, "");

    if (!target_str || !expr_str) {
        printf("Invalid command format. Use TARGET = EXPRESSION\n");
        return;
    }

    // Trim whitespace
    while(isspace(*target_str)) target_str++;
    char* end = target_str + strlen(target_str) - 1;
    while(end > target_str && isspace(*end)) *end-- = '\0';

    while(isspace(*expr_str)) expr_str++;
    end = expr_str + strlen(expr_str) - 1;
    while(end > expr_str && isspace(*end)) *end-- = '\0';

    int tx, ty;
    if (parse_cell_name(target_str, &tx, &ty) != 0) {
        printf("Invalid target cell: %s\n", target_str);
        return;
    }

    // Check if it's a simple assignment A0 = 5
    char* endptr;
    double val = strtod(expr_str, &endptr);
    if (*endptr == '\0') {
        cell_create(matrix, tx, ty, (CellContent){.dvalue = val}, 3);
        return;
    }

    // Check for functions like ADD(A0:A2)
    char* func_name = strtok(expr_str, "(");
    char* args_str = strtok(NULL, ")");
    if (func_name && args_str) {
        char* start_cell_str = strtok(args_str, ":");
        char* end_cell_str = strtok(NULL, "");

        int sx, sy, ex, ey;
        if (parse_cell_name(start_cell_str, &sx, &sy) == 0) {
            // Check if it's a range or a single cell
            if (end_cell_str) {
                if (parse_cell_name(end_cell_str, &ex, &ey) == 0) {
                    // Ensure cells exist or expand
                    cell_create(matrix, sx, sy, matrix->cells[sy][sx].content, matrix->cells[sy][sx].type);
                    cell_create(matrix, ex, ey, matrix->cells[ey][ex].content, matrix->cells[ey][ex].type);
                    cell_create(matrix, tx, ty, (CellContent){0}, 3); // result cell

                    if (strcmp(func_name, "ADD") == 0) {
                        if (sy == ey) {
                            add_row(matrix->cells, &matrix->cells[sy][sx], &matrix->cells[ey][ex], &matrix->cells[ty][tx]);
                        } else if (sx == ex) {
                            add_col(matrix->cells, &matrix->cells[sy][sx], &matrix->cells[ey][ex], &matrix->cells[ty][tx]);
                        } else {
                            printf("ADD only supports row or column ranges.\n");
                        }
                    } else if (strcmp(func_name, "AVG") == 0) {
                        if (sy == ey) {
                            avg_row(matrix->cells, &matrix->cells[sy][sx], &matrix->cells[ey][ex], &matrix->cells[ty][tx]);
                        } else if (sx == ex) {
                            avg_col(matrix->cells, &matrix->cells[sy][sx], &matrix->cells[ey][ex], &matrix->cells[ty][tx]);
                        } else {
                            printf("AVG only supports row or column ranges.\n");
                        }
                    } else {
                        printf("Range functions only support ADD and AVG for now.\n");
                    }
                } else {
                    printf("Invalid end cell: %s\n", end_cell_str);
                }
            } else {
                // Single cell or comma separated
                char* args_copy = strdup(args_str); // Use a copy because strtok is already being used on args_str
                strtok(args_copy, ","); // skip first arg
                char* next_arg = strtok(NULL, ",");
                if (next_arg) {
                    // Two cell operations: SUB(A0, B0)
                    while(isspace(*next_arg)) next_arg++;
                    if (parse_cell_name(next_arg, &ex, &ey) == 0) {
                        cell_create(matrix, sx, sy, matrix->cells[sy][sx].content, matrix->cells[sy][sx].type);
                        cell_create(matrix, ex, ey, matrix->cells[ey][ex].content, matrix->cells[ey][ex].type);
                        cell_create(matrix, tx, ty, (CellContent){0}, 3);

                        if (strcmp(func_name, "ADD") == 0) add(&matrix->cells[sy][sx], &matrix->cells[ey][ex], &matrix->cells[ty][tx]);
                        else if (strcmp(func_name, "SUB") == 0) sub(&matrix->cells[sy][sx], &matrix->cells[ey][ex], &matrix->cells[ty][tx]);
                        else if (strcmp(func_name, "MUL") == 0) mul(&matrix->cells[sy][sx], &matrix->cells[ey][ex], &matrix->cells[ty][tx]);
                        else if (strcmp(func_name, "DIV") == 0) divc(&matrix->cells[sy][sx], &matrix->cells[ey][ex], &matrix->cells[ty][tx]);
                        else printf("Unknown function or incorrect arguments: %s\n", func_name);
                    }
                    free(args_copy);
                } else {
                     // Single cell copy A1 = B1
                     cell_create(matrix, sx, sy, matrix->cells[sy][sx].content, matrix->cells[sy][sx].type);
                     cell_create(matrix, tx, ty, matrix->cells[sy][sx].content, matrix->cells[sy][sx].type);
                     free(args_copy);
                }
            }
        } else {
            printf("Invalid start cell: %s\n", start_cell_str);
        }
    } else {
        printf("Invalid expression: %s\n", expr_str);
    }
}
