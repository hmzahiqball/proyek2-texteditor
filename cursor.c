#include <string.h>
#include "cursor.h"
#include "buffer.h"
#include "render.h"

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
        cursor_col = getLineLength(cursor_row);
    }
    adjust_viewport();
}

void move_right() {
    int len = getLineLength(cursor_row);

    if (cursor_col < len) {
        cursor_col++;
    } else if (cursor_row < total_lines - 1) {
        cursor_row++;
        cursor_col = 0;
    }
    adjust_viewport();
}

void move_up() {
    if (cursor_row > 0) {
        cursor_row--;
        int len = getLineLength(cursor_row);
        if (cursor_col > len) cursor_col = len;
    }
    adjust_viewport();
}

void move_down() {
    if (cursor_row < total_lines - 1) {
        cursor_row++;
        int len = getLineLength(cursor_row);
        if (cursor_col > len) cursor_col = len;
    }
    adjust_viewport();
}

void adjust_viewport() {
    if (cursor_row < view_row_offset) {
        view_row_offset = cursor_row;
    } 
    else if (cursor_row >= view_row_offset + SCREEN_HEIGHT) {
        view_row_offset = cursor_row - SCREEN_HEIGHT + 1;
    }
}