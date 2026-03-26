#include <stdio.h>
#include <string.h>
#include "render.h"
#include "buffer.h"
#include "cursor.h"

// Menggambar ulang isi buffer ke layar dan menampilkan posisi kursor.
void renderScreen(char buffer[MAX_ROW][MAX_COL], int rows) {
    printf("\033[H\033[2J"); 

    if (rows == 0) {
        printf("NULL\n");
    } else {
        int i, j;
        for (i = 0; i < rows; i++) {
            if (i == cursor_row) {
                int len = strlen(buffer[i]);
                for (j = 0; j <= len; j++) {
                    if (j == cursor_col) {
                        printf("|");
                    }
                    if (j < len) {
                        printf("%c", buffer[i][j]);
                    }
                }
                printf("\n");
            } else {
                printf("%s\n", buffer[i]); 
            }
        }
    }
}

