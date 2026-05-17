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
    // ANTI-FLICKER: Pindahkan kursor ke pojok kiri atas (1,1) tanpa system("cls")
    printf("\033[H"); 

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
        // \033[K membersihkan sisa baris ke kanan agar tidak membekas
        printf("%s\033[K\n", current->line); 
        current = current->next;
        printed_lines++;
    }

    // Bersihkan sisa layar ke bawah jika isi file lebih pendek dari SCREEN_HEIGHT
    while (printed_lines < SCREEN_HEIGHT) 
    {
        printf("\033[K\n");
        printed_lines++;
    }

    // STATUS BAR
    printf("--------------------------------------------------\033[K\n");
    if (is_modified == 1) 
    {
        printf("[Unsaved Changes] | File: %s | Total Baris: %d\033[K\n", current_filename, total_lines);
    } 
    else 
    {
        printf("[Saved] | File: %s | Total Baris: %d\033[K\n", current_filename, total_lines);
    }
    printf("-------------------------------------------------------\n");
    
    printf("Posisi: Baris %d, Kolom %d | Ctrl+S: Save | Ctrl+A: Save As | ESC: Menu\033[K\n", 
       cursor_row + 1, cursor_col + 1);

    if (show_message) 
    {
        printf("\n%s\033[K", bottom_message); 
    }
    else
    {
        printf("\n\033[K");
    }

    // PENEMPATAN KURSOR TERMINAL SECARA DINAMIS
    if (input_mode) 
    {
        // Posisikan kursor tepat di ujung pesan prompt input bawah
        int msg_line = printed_lines + 7; 
        char *last_line = strrchr(bottom_message, '\n');
        int col = last_line ? strlen(last_line + 1) + 1 : strlen(bottom_message) + 1;
        
        // Geser kursor terminal fisik ke baris input bawah
        printf("\033[%d;%dH", msg_line, col);
    } 
    else 
    {
        // Pastikan kursor terminal kembali ke posisi teks editor utama saat mengetik biasa
        printf("\033[%d;%dH", cursor_row - view_row_offset + 1, cursor_col + 1);
    }

    // 7. DOUBLE-BUFFERING TRICK: Pastikan semua perintah kursor di atas dieksekusi secara instan
    fflush(stdout); 
}
