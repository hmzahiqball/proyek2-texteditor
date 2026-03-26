#include <string.h>
#include "buffer.h"

char text_buffer[MAX_ROW][MAX_COL];
int total_lines = 0;

// Inisialisasi posisi awal
int cursor_row = 0;
int cursor_col = 0;

void limitCursorBounds() {
    // Cegah kursor keluar dari batas baris (row)
    if (cursor_row < 0) cursor_row = 0;
    if (cursor_row >= MAX_ROW) cursor_row = MAX_ROW - 1;

    // Cegah kursor keluar dari batas kolom (col)
    if (cursor_col < 0) cursor_col = 0;
    if (cursor_col >= MAX_COL - 1) cursor_col = MAX_COL - 2; // -2 agar ada ruang untuk '\0'
}

void initBuffer() {
    int i;
    for (i = 0; i < MAX_ROW; i++) {
        text_buffer[i][0] = '\0';
    }
    total_lines = 0;

    // Reset posisi kursor saat buffer diinisialisasi
    cursor_row = 0;
    cursor_col = 0;
}

void appendLine(const char *input) {
    if (total_lines < MAX_ROW) {
        // Menyalin input user ke baris yang tersedia saat ini
        strncpy(text_buffer[total_lines], input, MAX_COL - 1);
        
        // Memastikan string diakhiri dengan null terminator (safe practice)
        text_buffer[total_lines][MAX_COL - 1] = '\0';
        total_lines++;

        // (Nantinya appendLine akan diganti dengan fungsi ketik karakter)
        cursor_row = total_lines;
        cursor_col = 0;
        limitCursorBounds();
    }
}
void insert_char(char c) {
    // Dapatkan panjang string di baris saat ini
    int len = strlen(text_buffer[cursor_row]);

    // Pastikan masih ada ruang di baris ini (MAX_COL - 1 untuk null terminator)
    if (len < MAX_COL - 1) {
        
        // Geser karakter ke kanan (Shift Right)
        int i;
        for (i = len; i >= cursor_col; i--) {
            text_buffer[cursor_row][i + 1] = text_buffer[cursor_row][i];
        }

        // Sisipkan karakter baru di posisi kursor
        text_buffer[cursor_row][cursor_col] = c;

        cursor_col++;

        // Update total_lines jika kita mengetik di baris yang benar-benar baru
        if (cursor_row >= total_lines) {
            total_lines = cursor_row + 1;
        }

        // 7. Jaring pengaman batas kursor
        limitCursorBounds();
    }
}

void delete_char() {
    // Jika kursor tidak berada di awal baris
    if (cursor_col > 0) {
        int len = strlen(text_buffer[cursor_row]);
        int i;
        
        // Geser karakter ke kiri untuk menimpa karakter sebelum kursor
        // Loop ini juga akan menggeser null terminator ('\0') di akhir string
        for (i = cursor_col; i <= len; i++) {
            text_buffer[cursor_row][i - 1] = text_buffer[cursor_row][i];
        }
        
        // Mundurkan kursor
        cursor_col--;
    } 
    // Jika kursor di awal baris, tapi bukan baris paling atas
    else if (cursor_row > 0) {
        int prev_len = strlen(text_buffer[cursor_row - 1]);
        int curr_len = strlen(text_buffer[cursor_row]);

        // Pastikan baris sebelumnya memiliki ruang yang cukup untuk digabung
        if (prev_len + curr_len < MAX_COL - 1) {
            
            // Salin isi baris saat ini ke akhir baris sebelumnya
            strcat(text_buffer[cursor_row - 1], text_buffer[cursor_row]);
            
            // Geser semua baris di bawahnya naik satu tingkat (Shift Up)
            int i;
            for (i = cursor_row; i < total_lines - 1; i++) {
                strcpy(text_buffer[i], text_buffer[i + 1]);
            }
            
            // Bersihkan sisa baris paling bawah setelah digeser
            text_buffer[total_lines - 1][0] = '\0';
            
            // Update total lines dan posisi kursor
            total_lines--;
            cursor_row--;
            cursor_col = prev_len; // Kursor berada di titik sambung
        }
    }
    
    // Pastikan kursor tetap aman
    limitCursorBounds();
}

void clearBuffer() {
    initBuffer();
}