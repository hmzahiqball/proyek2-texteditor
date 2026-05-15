#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "cursor.h"
#include "buffer.h"
#include "render.h"

// Variabel global untuk posisi cursor
int cursor_row = 0;
int cursor_col = 0;

LineNode *current_line = NULL;

// Inisialisasi cursor ke posisi awal (0,0)
void initCursor() {
    cursor_row = 0;
    cursor_col = 0;

    current_line = head;
}

// Membatasi posisi cursor agar tetap dalam batas buffer
void limitCursorBounds() {
    if (total_lines == 0) {
        cursor_row = 0;
        cursor_col = 0;
        current_line = NULL;
        return;
    }

    if (cursor_row < 0)
        cursor_row = 0;

    if (cursor_row >= total_lines)
        cursor_row = total_lines - 1;

    if (current_line == NULL)
        current_line = head;

    if (cursor_col < 0)
        cursor_col = 0;

    if (cursor_col > current_line->length)
        cursor_col = current_line->length;
}

// Mengatur cursor ke akhir dokumen
void set_cursor_to_end() {
    if (tail != NULL) {
        current_line = tail;

        cursor_row = total_lines - 1;
        cursor_col = tail->length;
    } else {
        current_line = NULL;

        cursor_row = 0;
        cursor_col = 0;
    }

    limitCursorBounds();
}

// Gerakan cursor ke kiri
void move_left() {
    if (!current_line) return;

    if (cursor_col > 0) {
        cursor_col--;
    } 
    else if (current_line->prev) {
        current_line = current_line->prev;

        cursor_row--;
        cursor_col = current_line->length;
    }

    adjust_viewport();
}

// Gerakan cursor ke kanan
void move_right() {
    if (!current_line) return;

    if (cursor_col < current_line->length) {
        cursor_col++;
    } 
    else if (current_line->next) {
        current_line = current_line->next;

        cursor_row++;
        cursor_col = 0;
    }

    adjust_viewport();
}

// Gerakan cursor ke atas
void move_up() {
    if (current_line && current_line->prev) {
        current_line = current_line->prev;

        cursor_row--;

        if (cursor_col > current_line->length) {
            cursor_col = current_line->length;
        }
    }

    adjust_viewport();
}

// Gerakan cursor ke bawah
void move_down() {
    if (current_line && current_line->next) {
        current_line = current_line->next;

        cursor_row++;

        if (cursor_col > current_line->length) {
            cursor_col = current_line->length;
        }
    }

    adjust_viewport();
}

// Menyesuaikan viewport berdasarkan posisi cursor
void adjust_viewport() {
    if (total_lines <= 0) {
        view_row_offset = 0;
        return;
    }

    if (cursor_row < view_row_offset) {
        view_row_offset = cursor_row;
    }
    else if (cursor_row >= view_row_offset + SCREEN_HEIGHT) {
        view_row_offset = cursor_row - SCREEN_HEIGHT + 1;
    }

    if (view_row_offset < 0) {
        view_row_offset = 0;
    }
}