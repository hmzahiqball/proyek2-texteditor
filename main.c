#include <stdio.h>
#include <string.h>
#include "recovery.h"

int main() {
    char text[100] = "";

    checkRecovery(text);

    // 🔥 TAMBAHKAN INI
    printf("Isi text saat ini:\n%s\n", text);
    while (1) {
        char input[50];

        printf("> ");
        fgets(input, sizeof(input), stdin);

        // hapus newline
        input[strcspn(input, "\n")] = 0;

        if (strcmp(input, "exit") == 0) {
            break;
        }

        // gabungkan ke text utama
        strcat(text, input);
        strcat(text, "\n");

        // 🔥 INI INTINYA (trigger recovery)
        writeRecovery(text);
    }

    return 0;
}