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
int view_col_offset = 0; // Mengendalikan pergeseran jendela visual ke kanan-kiri

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
    printf("\033[H\033[J"); // Bersihkan layar menu tanpa flicker
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
    printf("\033[H\033[J");
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
    printf("\033[H\033[J"); 
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
    // ANTI-FLICKER: Hanya kembali ke pojok kiri atas (0,0) tanpa menghapus layar hitam
    printf("\033[H"); 

    // TRAVERSE DLL: Cari node awal berdasarkan scroll vertikal saat ini
    LineNode *current = head;
    int i = 0;
    while (i < view_row_offset && current != NULL) 
    {
        current = current->next;
        i = i + 1;
    }

    // 1. CETAK TEKS VIEWPORT (ADOPRESI HORIZONTAL SCROLLING)
    int printed_lines = 0;
    while (current != NULL && printed_lines < SCREEN_HEIGHT) 
    {
        int line_len = strlen(current->line);
        
        // Jika panjang teks baris melebihi offset geser jendela saat ini
        if (line_len > view_col_offset) 
        {
            // Cetak dimulai dari karakter ke-view_col_offset, potong sepanjang SCREEN_WIDTH (80)
            printf("%.*s\033[K\n", SCREEN_WIDTH, current->line + view_col_offset);
        } 
        else 
        {
            // Jika baris kosong atau posisinya di luar offset, cetak baris kosong bersih
            printf("\033[K\n");
        }
        
        current = current->next;
        printed_lines++;
    }

    // Bersihkan sisa layar ke bawah jika isi file lebih pendek dari SCREEN_HEIGHT
    while (printed_lines < SCREEN_HEIGHT) 
    {
        printf("\033[K\n");
        printed_lines++;
    }

    // 2. STATUS BAR (UX Polish Tania: Konsisten Tegak Lurus 80 Kolom)
    printf("========================================================================\033[K\n");
    if (is_modified == 1) {
        printf(" [UNSAVED CHANGES] ");
    } else {
        printf(" [SAVED]           ");
    }
    printf("| Berkas: %s | Total: %d baris\033[K\n", current_filename, total_lines);
    printf("========================================================================\033[K\n");
    
    // Baris Posisi Kursor & Shortcut Navigasi
    printf(" Posisi: Baris %d, Kolom %d | Ctrl+S: Simpan | Ctrl+A: Save As | ESC: Menu\033[K\n", 
           cursor_row + 1, cursor_col + 1);

    // 3. CETAK PROMPT AREA PESAN BAWAH
    if (show_message) 
    {
        printf("\n%s\033[K", bottom_message); 
    }
    else
    {
        printf("\n\033[K"); 
    }

    fflush(stdout);

    // 4. PENEMPATAN KURSOR TERMINAL SECARA DINAMIS (Kunci Koordinat Absolut Windows API)
    if (input_mode) 
    {
        // Posisi baris input nama file: Viewport (20) + Status Bar (4) + Jarak Newline (2)
        int msg_line = SCREEN_HEIGHT + 6; 
        
        char *last_line = strrchr(bottom_message, '\n');
        int col = last_line ? strlen(last_line + 1) + 1 : strlen(bottom_message) + 1;
        
        setCursorPosition(msg_line, col);
    } 
    else 
    {
        // Posisi kursor mode ketik dihitung relatif terhadap offset jendela saat ini (row_offset dan col_offset)
        int visual_row = cursor_row - view_row_offset + 1;
        int visual_col = cursor_col - view_col_offset + 1;
        
        setCursorPosition(visual_row, visual_col);
    }

    fflush(stdout); // Semburkan data ke layar terminal secara serentak
}
