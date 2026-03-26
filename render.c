#include <stdio.h>
#include <string.h>
#include "render.h"
#include "buffer.h"

void renderScreen(char buffer[MAX_ROW][MAX_COL], int rows) {
    // Membersihkan layar dan kembali ke posisi atas (0,0)
    printf("\033[H\033[2J"); 

    // Jika buffer benar-benar kosong (awal program)
    if (rows == 0) {
        printf("|\n");
    } else {
        int i, j;
        for (i = 0; i < rows; i++) {
            // Jika ini adalah baris tempat kursor berada
            if (i == cursor_row) {
                int len = strlen(buffer[i]);
                
                // Looping karakter per karakter sampai batas null terminator (len)
                for (j = 0; j <= len; j++) {
                    // Cetak kursor SEBELUM mencetak karakter di indeks ini
                    if (j == cursor_col) {
                        printf("|");
                    }
                    
                    // Cetak karakter aslinya (jika belum mencapai akhir string)
                    if (j < len) {
                        printf("%c", buffer[i][j]);
                    }
                }
                printf("\n");
            } 
            // Jika ini BUKAN baris kursor, cetak string secara normal sekaligus
            else {
                printf("%s\n", buffer[i]); 
            }
        }
    }

    printf("\n--- Editor Saw<git> | Cursor: (%d, %d) | Baris: %d ---\n", cursor_row, cursor_col, rows);
}

