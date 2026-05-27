#include "Parser.h"
#include "Functions.h"
#include "utils.h"
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

static double get_value(Matrix* matrix, char* str, short* type) {
    char* s = str;
    while(isspace(*s)) s++;
    char* end = s + strlen(s) - 1;
    while(end > s && isspace(*end)) *end-- = '\0';

    int x, y;
    if (parse_cell_name(s, &x, &y) == 0) {
        // Expand if needed
        cell_create(matrix, x, y, matrix->cells[y][x].content, matrix->cells[y][x].type);
        Cell* cell = &matrix->cells[y][x];
        *type = cell->type;
        if (cell->type == 1) return (double)cell->content.value;
        if (cell->type == 2) return (double)cell->content.fvalue;
        if (cell->type == 3) return cell->content.dvalue;
        return 0;
    }

    char* endptr;
    double val = strtod(s, &endptr);
    if (endptr != s && (*endptr == '\0' || isspace(*endptr))) {
        *type = 3;
        return val;
    }
    return 0;
}

void process_command(Matrix* matrix, char* command) {
    char cmd_copy[256];
    strncpy(cmd_copy, command, sizeof(cmd_copy)-1);
    cmd_copy[sizeof(cmd_copy)-1] = '\0';
    char* target_str = strtok(cmd_copy, "=");
    char* expr_str = strtok(NULL, "");

    if (!target_str || !expr_str) {
        printf("Invalid command format. Use TARGET = EXPRESSION\n");
        return;
    }

    while(isspace(*target_str)) target_str++;
    char* end_t = target_str + strlen(target_str) - 1;
    while(end_t > target_str && isspace(*end_t)) *end_t-- = '\0';

    while(isspace(*expr_str)) expr_str++;
    char* end_e = expr_str + strlen(expr_str) - 1;
    while(end_e > expr_str && isspace(*end_e)) *end_e-- = '\0';

    int tx, ty;
    if (parse_cell_name(target_str, &tx, &ty) != 0) {
        printf("Invalid target cell: %s\n", target_str);
        return;
    }

    // Check for math operators (+, -, *, /)
    char operators[] = {'+', '-', '*', '/'};
    char* op_ptr = containsAny(expr_str, strlen(expr_str), operators, 4);

    if (op_ptr) {
        char operator = *op_ptr;
        *op_ptr = '\0';
        char left_str[256];
        char right_str[256];
        strncpy(left_str, expr_str, sizeof(left_str)-1);
        left_str[sizeof(left_str)-1] = '\0';
        strncpy(right_str, op_ptr + 1, sizeof(right_str)-1);
        right_str[sizeof(right_str)-1] = '\0';

        short type1 = 0, type2 = 0;
        double val1 = get_value(matrix, left_str, &type1);
        double val2 = get_value(matrix, right_str, &type2);
        double result = 0;

        switch(operator) {
            case '+': result = val1 + val2; break;
            case '-': result = val1 - val2; break;
            case '*': result = val1 * val2; break;
            case '/':
                if (val2 != 0) result = val1 / val2;
                else { printf("Division by zero\n"); return; }
                break;
            default:
                break;
        }
        cell_create(matrix, tx, ty, (CellContent){.dvalue = result}, 3);
        return;
    }

    // Check if it's a simple assignment A0 = 5
    char* endptr;
    double val = strtod(expr_str, &endptr);
    if (endptr != expr_str && (*endptr == '\0' || isspace(*endptr))) {
        cell_create(matrix, tx, ty, (CellContent){.dvalue = val}, 3);
        return;
    }

    // Check for functions like ADD(A0:A2)
    char* expr_copy = strdup(expr_str);
    char* func_name = strtok(expr_copy, "(");
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
        // Fallback for single cell copy if it didn't look like a function
        int sx, sy;
        if (parse_cell_name(expr_str, &sx, &sy) == 0) {
            cell_create(matrix, sx, sy, matrix->cells[sy][sx].content, matrix->cells[sy][sx].type);
            cell_create(matrix, tx, ty, matrix->cells[sy][sx].content, matrix->cells[sy][sx].type);
        } else {
            printf("Invalid expression: %s\n", expr_str);
        }
    }
    free(expr_copy);
}


