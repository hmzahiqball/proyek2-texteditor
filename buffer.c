#include <string.h>
#include <stdlib.h>
#include "buffer.h"
#include "cursor.h"

char text_buffer[MAX_ROW][MAX_COL];
int total_lines = 0;
int line_length[MAX_ROW];

void initBuffer() {
    int i, j;
    for (i = 0; i < MAX_ROW; i++) {
        for (j = 0; j < MAX_COL; j++) {
            text_buffer[i][j] = '\0';
        }
        line_length[i] = 0;
    }
    total_lines = 0;
}

void clearBuffer() {
    int i, j;
    for (i = 0; i < MAX_ROW; i++) {
        for (j = 0; j < MAX_COL; j++) {
            text_buffer[i][j] = '\0';
        }
        line_length[i] = 0;
    }
    total_lines = 0;
}

void appendLine(const char *input) {
    if (total_lines >= MAX_ROW) return;

    int len = strlen(input);
    if (len > MAX_COL - 1) len = MAX_COL - 1; // Batasi panjang

    strncpy(text_buffer[total_lines], input, len);
    text_buffer[total_lines][len] = '\0';

    line_length[total_lines] = len;
    total_lines++;
}

void insert_char(char c) {
    int len = line_length[cursor_row];

    if (len >= MAX_COL - 1) return; // Tidak bisa insert jika sudah penuh

    // Geser karakter ke kanan mulai dari cursor_col
    memmove(
        &text_buffer[cursor_row][cursor_col + 1],
        &text_buffer[cursor_row][cursor_col],
        len - cursor_col + 1
    );

    text_buffer[cursor_row][cursor_col] = c;
    line_length[cursor_row]++;

    cursor_col++;

    if (cursor_row >= total_lines) {
        total_lines = cursor_row + 1;
    }

    limitCursorBounds();
    adjust_viewport();
}

void delete_char() {
    if (cursor_col > 0) {
        int len = line_length[cursor_row];

        // Geser karakter ke kiri mulai dari cursor_col
        memmove(
            &text_buffer[cursor_row][cursor_col - 1],
            &text_buffer[cursor_row][cursor_col],
            len - cursor_col + 1
        );

        line_length[cursor_row]--;

        cursor_col--;
    }

    limitCursorBounds();
    adjust_viewport();
}

void insert_newline() {
    if (total_lines >= MAX_ROW) return;

    int len = line_length[cursor_row];

    // Salin bagian setelah cursor ke baris baru
    int new_line_len = len - cursor_col;
    if (new_line_len > 0) {
        memcpy(text_buffer[cursor_row + 1], &text_buffer[cursor_row][cursor_col], new_line_len);
        text_buffer[cursor_row + 1][new_line_len] = '\0';
    } else {
        text_buffer[cursor_row + 1][0] = '\0';
    }

    // Set akhir baris lama
    text_buffer[cursor_row][cursor_col] = '\0';
    line_length[cursor_row] = cursor_col;

    // Geser baris-baris di bawah
    int i;
    for (i = total_lines; i > cursor_row + 1; i--) {
        memcpy(text_buffer[i], text_buffer[i - 1], MAX_COL);
        line_length[i] = line_length[i - 1];
    }

    line_length[cursor_row + 1] = new_line_len;

    total_lines++;

    cursor_row++;
    cursor_col = 0;

    limitCursorBounds();
    adjust_viewport();
}

int getLineLength(int row) {
    if (row < 0 || row >= total_lines) return 0;
    return line_length[row];
}

char getCharAt(int row, int col) {
    if (row < 0 || row >= total_lines) return '\0';
    if (col < 0 || col >= line_length[row]) return '\0';
    return text_buffer[row][col];
}