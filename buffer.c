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

        // --- TAMBAHAN TAHAP 1: Sesuaikan kursor saat fungsi lama ini dipanggil ---
        // (Nantinya appendLine akan diganti dengan fungsi ketik karakter)
        cursor_row = total_lines;
        cursor_col = 0;
        limitCursorBounds();
    }
}

void clearBuffer() {
    initBuffer();
}