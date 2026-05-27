#include <stdio.h>
#include <string.h>
#include "Cell.h"
#include "Functions.h"
#include "Parser.h"

int main(void) {
    Matrix matrix;
    init(&matrix, 6, 6);

    char buffer[256];
    printf("Commands: A0 = 5, A3 = ADD(A0:A2), exit\n");

    while (1) {
        printf("> ");
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) break;
        buffer[strcspn(buffer, "\n")] = '\0';

        if (strcmp(buffer, "exit") == 0) break;
        if (strlen(buffer) == 0) continue;

        process_command(&matrix, buffer);
        print_matrix(&matrix);
    }

    return 0;
}
