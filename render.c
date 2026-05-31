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
// Fungsi utama untuk menggambar ulang seluruh tampilan editor ke layar terminal
void renderScreen(void *unused_buffer, int unused_rows) 
{
    printf("\033[H");  // Pindahkan kursor ke pojok kiri atas (0,0) TANPA MENGHAPUS LAYAR (Anti-Flicker)

    // TRAVERSE DLL: Cari node awal berdasarkan scroll saat ini
    LineNode *current = head;
    int i = 0;
    while (i < view_row_offset && current != NULL) 
    {
        current = current->next;
        i = i + 1;
    }

    // VARIABLES UNTUK MENGHITUNG TEKS WRAPPING SECARA PRESISI
    int printed_lines = 0;
    int visual_cursor_row = 1; 
    int visual_cursor_col = 1; 
    
    // 1. CETAK TEKS VIEWPORT (Mekanisme Pemotongan Karakter per SCREEN_WIDTH + Sapu Bersih \033[K)
    int current_row_idx = view_row_offset;
    while (current != NULL && printed_lines < SCREEN_HEIGHT) 
    {
        int line_len = strlen(current->line);
        
        if (line_len == 0) {
            if (current_row_idx == cursor_row) {
                visual_cursor_row = printed_lines + 1;
                visual_cursor_col = 1;
            }
            printf("\033[K\n"); // Sapu bersih baris kosong ke kanan sebelum turun
            printed_lines++;
        } else {
            int chars_printed = 0;
            while (chars_printed < line_len && printed_lines < SCREEN_HEIGHT) {
                int rem = line_len - chars_printed;
                int to_print = (rem > SCREEN_WIDTH) ? SCREEN_WIDTH : rem;
                
                // Cek koordinat kursor di dalam segmen lipatan baris
                if (current_row_idx == cursor_row && cursor_col >= chars_printed && cursor_col < chars_printed + to_print) {
                    visual_cursor_row = printed_lines + 1;
                    visual_cursor_col = (cursor_col - chars_printed) + 1;
                }
                if (current_row_idx == cursor_row && cursor_col == chars_printed + to_print && to_print < SCREEN_WIDTH) {
                    visual_cursor_row = printed_lines + 1;
                    visual_cursor_col = to_print + 1;
                }

                // Cetak segmen teks diikuti \033[K untuk menghapus sisa karakter lama di baris tersebut
                printf("%.*s\033[K\n", to_print, current->line + chars_printed);
                
                chars_printed += to_print;
                printed_lines++;
            }
            
            if (current_row_idx == cursor_row && cursor_col == line_len && line_len % SCREEN_WIDTH == 0) {
                visual_cursor_row = printed_lines + 1;
                visual_cursor_col = 1;
            }
        }

        current = current->next;
        current_row_idx++;
    }

    // 2. PEMBERSIH AREA SISA LAYAR BAWAH (Sapu bersih per baris agar bebas flicker)
    while (printed_lines < SCREEN_HEIGHT) 
    {
        printf("\033[K\n"); 
        printed_lines = printed_lines + 1;
    }

    // 3. STATUS BAR (UX Polish - Konsisten 80 Kolom)
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

    // 4. CETAK PROMPT AREA PESAN BAWAH
    if (show_message) 
    {
        printf("\n%s\033[K", bottom_message); 
    }
    else
    {
        printf("\n\033[K"); 
    }

    fflush(stdout);

    // 5. PENEMPATAN KURSOR TERMINAL SECARA DINAMIS (Menggunakan Windows API bawaan)
    if (input_mode) 
    {
        int msg_line = SCREEN_HEIGHT + 6; 
        char *last_line = strrchr(bottom_message, '\n');
        int col = last_line ? strlen(last_line + 1) + 1 : strlen(bottom_message) + 1;
        setCursorPosition(msg_line, col);
    } 
    else 
    {
        // Kursor visual dilempar secara presisi berdasarkan pelipatan karakter
        setCursorPosition(visual_cursor_row, visual_cursor_col);
    }

    fflush(stdout); 
}
