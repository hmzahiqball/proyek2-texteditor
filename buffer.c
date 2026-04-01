#include <string.h>
#include "buffer.h"
#include "cursor.h"

char text_buffer[MAX_ROW][MAX_COL];
int total_lines = 0;

void initBuffer() {
    int i;
    for (i = 0; i < MAX_ROW; i++) {
        text_buffer[i][0] = '\0';
    }
    total_lines = 0;
}

void clearBuffer() {
    int i;
    for (i = 0; i < MAX_ROW; i++) {
        text_buffer[i][0] = '\0';
    }
    total_lines = 0;
}

void appendLine(const char *input) {
    if (total_lines >= MAX_ROW) return;

    strncpy(text_buffer[total_lines], input, MAX_COL - 1);
    text_buffer[total_lines][MAX_COL - 1] = '\0';

    total_lines++;
}

// Menyisipkan satu karakter di posisi kursor dan menggeser teks di kanannya.
void insert_char(char c) {
    int len = strlen(text_buffer[cursor_row]);
    if (len < MAX_COL - 1) {
        int i;
        for (i = len; i >= cursor_col; i--) {
            text_buffer[cursor_row][i + 1] = text_buffer[cursor_row][i];
        }

        text_buffer[cursor_row][cursor_col] = c;

        cursor_col++;

        if (cursor_row >= total_lines) {
            total_lines = cursor_row + 1;
        }
        limitCursorBounds();
    }
}

// Menghapus karakter sebelum kursor, atau menggabungkan dua baris jika di awal baris.
void delete_char() {
    if (cursor_col > 0) {
        int len = strlen(text_buffer[cursor_row]);
        int i;
        for (i = cursor_col; i <= len; i++) {
            text_buffer[cursor_row][i - 1] = text_buffer[cursor_row][i];
        }
        cursor_col--;
    } 
    else if (cursor_row > 0) {
        int prev_len = strlen(text_buffer[cursor_row - 1]);
        int curr_len = strlen(text_buffer[cursor_row]);

        if (prev_len + curr_len < MAX_COL - 1) {
            strcat(text_buffer[cursor_row - 1], text_buffer[cursor_row]);
            int i;
            for (i = cursor_row; i < total_lines - 1; i++) {
                strcpy(text_buffer[i], text_buffer[i + 1]);
            }
            text_buffer[total_lines - 1][0] = '\0';
            total_lines--;
            cursor_row--;
            cursor_col = prev_len;
        }
    }
    limitCursorBounds();
}

// Menyisipkan baris baru di bawah kursor dan memindahkan sisa teks ke baris berikutnya.
void insert_newline() {
    if (total_lines >= MAX_ROW) {
        return;
    }

    if (total_lines == 0) {
        total_lines = 1;
    }

    int i;
    for (i = total_lines; i > cursor_row; i--) {
        strcpy(text_buffer[i], text_buffer[i - 1]);
    }

    strcpy(text_buffer[cursor_row + 1], &text_buffer[cursor_row][cursor_col]);

    text_buffer[cursor_row][cursor_col] = '\0';

    total_lines++;
    cursor_row++;
    cursor_col = 0;

    limitCursorBounds();
}

// helper
int getLineLength(int row) {
    if (row < 0 || row >= total_lines) return 0;
    return strlen(text_buffer[row]);
}