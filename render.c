#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
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

// Helper function untuk set cursor position menggunakan Windows API
void setCursorPosition(int row, int col) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord;
    coord.X = col - 1;  // 0-based
    coord.Y = row - 1;  // 0-based
    SetConsoleCursorPosition(hConsole, coord);
}

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
    printf("Teknis      : Berbasis Double Linked List   \n");
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
    printf(" Ctrl + S : Untuk menyimpan perubahan file         \n");
    printf("            Save (Pada Menu Open) / Save As (Pada menu Create)\n");
    printf(" Ctrl + Q : Keluar dari Program                   \n");
    printf(" ESC      : Kembali ke Menu Utama                 \n");
    printf(" Panah    : Navigasi Kursor                       \n");
    printf("==================================================\n");
    printf("\nTekan sembarang tombol untuk kembali...");
}

// Fungsi utama untuk menggambar ulang seluruh tampilan editor ke layar terminal
void renderScreen(void *unused_buffer, int unused_rows) 
{
    system("cls");  // Clear screen untuk Windows compatibility

    // TRAVERSE DLL: Cari node awal berdasarkan scroll saat ini (Menggunakan WHILE)
    LineNode *current = head;
    int i = 0;
    while (i < view_row_offset && current != NULL) 
    {
        current = current->next;
        i++;
    }

    // CETAK TEKS VIEWPORT
    int printed_lines = 0;
    while (current != NULL && printed_lines < SCREEN_HEIGHT) 
    {
        // Cetak line content
        printf("%s\n", current->line); 
        current = current->next;
        printed_lines++;
    }

    // Bersihkan sisa layar ke bawah jika isi file lebih pendek dari SCREEN_HEIGHT
    while (printed_lines < SCREEN_HEIGHT) 
    {
        printf("\n");
        printed_lines++;
    }

    // STATUS BAR
    printf("--------------------------------------------------\n");
    if (is_modified == 1) 
    {
        printf("[Unsaved Changes] | File: %s | Total Baris: %d\n", current_filename, total_lines);
    } 
    else 
    {
        printf("[Saved] | File: %s | Total Baris: %d\n", current_filename, total_lines);
    }
    printf("--------------------------------------------------\n");
    
    printf("Posisi: Baris %d, Kolom %d | Ctrl+S: Save | Ctrl+A: Save As | ESC: Menu\n", 
       cursor_row + 1, cursor_col + 1);

    if (show_message) 
    {
        printf("\n%s", bottom_message); 
    }
    else
    {
        printf("\n");
    }

    fflush(stdout);

    // PENEMPATAN KURSOR TERMINAL SECARA DINAMIS
    if (input_mode) 
    {
        // Kunci koordinat baris prompt di terminal (SCREEN_HEIGHT + 6 baris komponen status bar)
        int msg_line = SCREEN_HEIGHT + 6; 
        
        // Hitung posisi cursor di akhir bottom_message
        // Jika ada newline, hitung dari setelah newline terakhir
        char *last_line = strrchr(bottom_message, '\n');
        int col = last_line ? strlen(last_line + 1) + 1 : strlen(bottom_message) + 1;
        
        // Gunakan Windows API untuk positioning yang reliable
        setCursorPosition(msg_line, col);
    } 
    else 
    {
        // Kembalikan kursor terminal ke posisi teks editor utama saat mode mengetik biasa
        setCursorPosition(cursor_row - view_row_offset + 1, cursor_col + 1);
    }

    fflush(stdout); // Paksa seluruh buffer karakter keluar bersamaan ke layar terminal
}
