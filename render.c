#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "render.h"
#include "buffer.h"
#include "cursor.h"


void renderMainMenu() {
    system("cls"); // Pastikan layar bersih
    printf("==================================================\n");
    printf("=========== Saw<git> | Text Editor ===============\n");
    printf("1. Open file\n");
    printf("2. Create File\n");
    printf("3. Info\n");
    printf("4. Help\n");
    printf("5. Quit\n");
    printf("==================================================\n");
    
    if (total_lines > 0) {
        printf("[!] Status: Buffer terisi (%d baris).\n", total_lines);
    }
    printf("\nSawgit> ");
}

// Menggambar ulang isi buffer ke layar dan menampilkan posisi kursor.
void renderScreen(char buffer[MAX_ROW][MAX_COL], int rows) {
    system("cls"); // Pakai ini agar lebih stabil di Windows dibanding kode ANSI

    if (rows == 0) 
	{
        // Jangan cetak NULL, biarkan kosong atau cetak baris kosong
        printf("\n"); 
    } 
	else 
	{
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

