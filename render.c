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
    printf(" Ctrl + S : Untuk menyimpan perubahan file         \n");
    printf("            Save (Pada Menu Open) / Save As (Pada menu Create)\n");
    printf(" Ctrl + Q : Keluar dari Program                   \n");
    printf(" ESC      : Kembali ke Menu Utama                 \n");
    printf(" Panah    : Navigasi Kursor                       \n");
    printf("==================================================\n");
    printf("\nTekan sembarang tombol untuk kembali...");
}

// Fungsi utama untuk menggambar ulang seluruh tampilan editor ke layar terminal
void renderScreen(char buffer[MAX_ROW][MAX_COL], int rows) 
{
    system("cls"); // Membersihkan layar terminal agar gambar yang baru tidak menumpuk

    // Menghitung batasan baris yang akan ditampilkan (Scrolling Logic)
    int start = view_row_offset; // Baris awal yang mulai terlihat di layar
    int end = view_row_offset + SCREEN_HEIGHT; // Batas akhir layar berdasarkan tinggi maksimal
    if (end > rows) end = rows; // Pastikan batas akhir tidak melebihi jumlah baris yang ada di buffer
    int visible_lines = end - start; // Menghitung berapa banyak baris teks yang benar-benar tercetak

    // Melakukan perulangan untuk mencetak isi teks dari text_buffer ke layar
    int i;
    for (i = start; i < end; i++) 
    {
        printf("%s\n", buffer[i]); // Mencetak string tiap baris teks diikuti karakter baris baru
    }

    // Bagian pencetakan Status Bar (Elemen dekoratif di bawah teks)
    printf("\n--------------------------------------------------\n");
    if (is_modified == 1) // Cek apakah ada perubahan yang belum di-save
    {
        // Tampilkan status [Unsaved Changes] jika file sudah dimodifikasi
        printf("[Unsaved Changes] | File: %s | Baris: %d\n", current_filename, total_lines);
    } 
    else 
    {
        // Tampilkan status [Saved] jika file sudah aman tersimpan
        printf("[Saved] | File: %s | Baris: %d\n", current_filename, total_lines);
    }
    printf("--------------------------------------------------\n");
    
    // Mencetak informasi koordinat kursor dan petunjuk shortcut utama
    // \033[K digunakan untuk membersihkan sisa karakter di baris tersebut agar tidak berbayang
    printf("Posisi: Baris %d, Kolom %d | Ctrl+S: Save | ESC: Menu\033[K\n", 
       cursor_row + 1, cursor_col + 1);

    // Bagian untuk menampilkan Pesan Sistem (Bottom Message)
    if (show_message) 
    {
        // Mencetak pesan tambahan (misal: "Masukkan nama file" atau "Berhasil Simpan")
        printf("\n%s\033[K", bottom_message); 
    }

    // Logika Penempatan Kursor Terminal secara Dinamis (ANSI Escape Codes)
    if (input_mode) // Jika program sedang meminta input (seperti nama file)
    {
        // Menentukan baris terminal tempat kursor input berada (Teks + Status Bar + Spasi)
        int msg_line = visible_lines + 7; 

        // Mencari karakter Enter (\n) terakhir dalam pesan untuk menentukan kolom kursor
        char *last_line = strrchr(bottom_message, '\n');
        
        // Menghitung posisi kolom kursor agar tepat berada di ujung teks pesan
        int col = last_line ? strlen(last_line + 1) + 1 : strlen(bottom_message) + 1;

        // Memindahkan kursor terminal ke area pesan bawah menggunakan kode ANSI \033[Baris;KolomH
        printf("\033[%d;%dH", msg_line, col);
    } 
    else // Jika user sedang dalam mode mengetik teks biasa (Navigasi Editor)
    {
        // Memindahkan kursor terminal ke posisi baris dan kolom teks yang sedang diedit
        // cursor_row dikurangi offset agar posisi kursor sinkron dengan scroll layar
        printf("\033[%d;%dH", cursor_row - view_row_offset + 1, cursor_col + 1);
    }
}
