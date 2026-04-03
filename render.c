#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "render.h"
#include "buffer.h"
#include "cursor.h"

extern int is_modified;
extern char current_filename[256];
extern int total_lines;

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


void renderHelpScreen() {
    system("cls"); //
    printf("==================================================\n");
    printf("           BANTUAN SHORTCUT SAW<GIT>              \n");
    printf("==================================================\n");
    printf(" Ctrl + O : Untuk membuka file                    \n");
    printf(" Ctrl + N : Untuk membuka file baru               \n");
    printf(" Ctrl + I : Untuk membuka informasi aplikasi      \n");
    printf(" Ctrl + G : Untuk membuka informasi shortcut      \n");
    printf(" Ctrl + S : Simpan (Pada menu open / Save As ( Pada menu Create) \n");
    printf(" Ctrl + Q : Keluar dari Program                   \n");
    printf(" ESC      : Kembali ke Menu Utama                 \n");
    printf(" Panah    : Navigasi Kursor                       \n");
    printf("==================================================\n");
    printf("\nTekan sembarang tombol untuk kembali...");
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

    // Tampilkan status file dan posisi kursor di bagian bawah layar
    printf("\n--------------------------------------------------\n");
    if (is_modified == 1) {
        printf("[Unsaved Changes] | File: %s | Baris: %d\n", current_filename, total_lines);
    } else {
        printf("[Saved] | File: %s | Baris: %d\n", current_filename, total_lines);
    }
    printf("--------------------------------------------------\n");
    printf("Posisi: Baris %d, Kolom %d | Ctrl+S: Save | ESC: Menu\n", cursor_row + 1, cursor_col + 1);
}
