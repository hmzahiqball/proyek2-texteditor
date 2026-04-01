#include <string.h>
#include "cursor.h"
#include "buffer.h"

// Fungsi bantu untuk mengelola posisi kursor di dalam buffer teks.
int cursor_row = 0;
int cursor_col = 0;

void initCursor() {
    cursor_row = 0;
    cursor_col = 0;
}

void limitCursorBounds() {
    if (cursor_row < 0) cursor_row = 0;

    if (total_lines == 0) {
        cursor_row = 0;
        cursor_col = 0;
        return;
    }

    if (cursor_row >= total_lines) {
        cursor_row = total_lines - 1;
    }

    int len = getLineLength(cursor_row);

    if (cursor_col < 0) cursor_col = 0;
    if (cursor_col > len) cursor_col = len;
}

void set_cursor_to_end() {
    if (total_lines > 0) {
        cursor_row = total_lines - 1;
        cursor_col = getLineLength(cursor_row);
    } else {
        cursor_row = 0;
        cursor_col = 0;
    }

    limitCursorBounds();
}

void move_left() {
    if (cursor_col > 0) {
        cursor_col--;
    } else if (cursor_row > 0) {
        cursor_row--;
        cursor_col = strlen(text_buffer[cursor_row]);
    }
}

void move_right() {
    int len = strlen(text_buffer[cursor_row]);

    if (cursor_col < len) {
        cursor_col++;
    } else if (cursor_row < total_lines - 1) {
        cursor_row++;
        cursor_col = 0;
    }
}

void move_up() {
    if (cursor_row > 0) {
        cursor_row--;
        int len = strlen(text_buffer[cursor_row]);
        if (cursor_col > len) cursor_col = len;
    }
}

void move_down() {
    if (cursor_row < total_lines - 1) {
        cursor_row++;
        int len = strlen(text_buffer[cursor_row]);
        if (cursor_col > len) cursor_col = len;
    }
}