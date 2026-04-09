#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "render.h"
#include "buffer.h"
#include "cursor.h"


extern int is_modified;
extern char current_filename[256];
extern int total_lines;

int view_row_offset = 0;
int view_col_offset = 0;

char bottom_message[256] = "";
int show_message = 0;
int input_mode = 0;
int is_in_editor = 0;

void renderMainMenu() 
{
    system("cls"); // Pastikan layar bersih
    printf("==================================================\n");
    printf("=========== Saw<git> | Text Editor ===============\n");
    printf("1. Open file\n");
    printf("2. Create File\n");
    printf("3. Info\n");
    printf("4. Help\n");
    printf("5. Quit\n");
    printf("==================================================\n");
    
    if (total_lines > 0) 
	{
        printf("[!] Status: Buffer terisi (%d baris).\n", total_lines);
    }
    printf("\nSawgit> ");
}

void renderInfoScreen() 
{
    system("cls");
    printf("============================================\n");
    printf("========== INFO APLIKASI SAW<GIT> ==========\n");
    printf("============================================\n");
    printf("Versi       : 1.0                           \n");
    printf("Pengembang  : Putra, R.Neysa, Tania         \n");
    printf("--------------------------------------------\n");
    printf("Teknis      : Berbasis Console 2D Array     \n");
    printf("              (Max: %dx%d Character)\n", MAX_ROW, MAX_COL);
    printf("============================================\n");
    printf("\nTekan sembarang tombol untuk kembali..."     );
}


void renderHelpScreen() 
{
    system("cls"); 
    printf("==================================================\n");
    printf("           BANTUAN SHORTCUT SAW<GIT>              \n");
    printf("==================================================\n");
    printf(" Ctrl + O : Untuk membuka file                    \n");
    printf(" Ctrl + N : Untuk membuka file baru               \n");
    printf(" Ctrl + I : Untuk membuka informasi aplikasi      \n");
    printf(" Ctrl + G : Untuk membuka informasi shortcut      \n");
    printf(" Ctrl + S : Untuk menyimpan perbuhan file         \n");
    printf(" 			Save (Pada Menu Open) / Save As (Pada menu Create)     \n");
    printf(" Ctrl + Q : Keluar dari Program                   \n");
    printf(" ESC      : Kembali ke Menu Utama                 \n");
    printf(" Panah    : Navigasi Kursor                       \n");
    printf("==================================================\n");
    printf("\nTekan sembarang tombol untuk kembali...");
}

// Menggambar ulang isi buffer ke layar dan menampilkan posisi kursor.
void renderScreen(char buffer[MAX_ROW][MAX_COL], int rows) 
{
    system("cls");

    int start = view_row_offset;
    int end = view_row_offset + SCREEN_HEIGHT;
    if (end > rows) end = rows;
    int visible_lines = end - start;

    int i;
    for (i = start; i < end; i++) 
	{
        printf("%s\n", buffer[i]);
    }

    // status bar
    printf("\n--------------------------------------------------\n");
    if (is_modified == 1) 
	{
        printf("[Unsaved Changes] | File: %s | Baris: %d\n", current_filename, total_lines);
    } 
	else 
	{
        printf("[Saved] | File: %s | Baris: %d\n", current_filename, total_lines);
    }
    printf("--------------------------------------------------\n");
    printf("Posisi: Baris %d, Kolom %d | Ctrl+S: Save | ESC: Menu\033[K\n", 
       cursor_row + 1, cursor_col + 1);

	// CETAK PESAN DI SINI (Di bawah garis kursor)
	if (show_message) 
	{
	    printf("\n%s\033[K", bottom_message); 
	}

    if (input_mode) 
	{
    // posisi message = setelah semua UI
    int msg_line = visible_lines + 7;

    // ambil panjang teks terakhir (buat posisi kolom)
    char *last_line = strrchr(bottom_message, '\n');
    int col = last_line ? strlen(last_line + 1) + 1 : strlen(bottom_message) + 1;

    printf("\033[%d;%dH", msg_line, col);
	} 
	else 
	{
    printf("\033[%d;%dH", cursor_row - view_row_offset + 1, cursor_col + 1);
	}
}
