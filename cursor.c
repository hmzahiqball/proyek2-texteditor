#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "cursor.h"
#include "buffer.h"
#include "render.h"

// Variabel global untuk posisi cursor
int cursor_row = 0;
int cursor_col = 0;

// Inisialisasi cursor ke posisi awal (0,0)
void initCursor() {
    cursor_row = 0;
    cursor_col = 0;
}

// Membatasi posisi cursor agar tetap dalam batas buffer
void limitCursorBounds() {
    if (total_lines < 0 || total_lines > MAX_ROW) {
        printf("Error: total_lines (%d) di luar batas [0, %d].\n", total_lines, MAX_ROW);
        return;
    }
    assert(total_lines >= 0 && total_lines <= MAX_ROW); // Error handling: total_lines dalam batas
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

// Mengatur cursor ke akhir dokumen
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

// Gerakan cursor ke kiri
void move_left() {
    if (cursor_row < 0 || cursor_row >= MAX_ROW) {
        printf("Error: cursor_row (%d) di luar batas [0, %d).\n", cursor_row, MAX_ROW);
        return;
    }
    if (cursor_col < 0 || cursor_col > MAX_COL) {
        printf("Error: cursor_col (%d) di luar batas [0, %d].\n", cursor_col, MAX_COL);
        return;
    }
    assert(cursor_row >= 0 && cursor_row < MAX_ROW); // Error handling: cursor_row dalam batas
    assert(cursor_col >= 0 && cursor_col <= MAX_COL); // Error handling: cursor_col dalam batas
    if (cursor_col > 0) {
        cursor_col--;
    } else if (cursor_row > 0) {
        cursor_row--;
        cursor_col = getLineLength(cursor_row);
    }
    adjust_viewport();
}

// Gerakan cursor ke kanan
void move_right() {
    if (cursor_row < 0 || cursor_row >= MAX_ROW) {
        printf("Error: cursor_row (%d) di luar batas [0, %d).\n", cursor_row, MAX_ROW);
        return;
    }
    if (cursor_col < 0 || cursor_col > MAX_COL) {
        printf("Error: cursor_col (%d) di luar batas [0, %d].\n", cursor_col, MAX_COL);
        return;
    }
    assert(cursor_row >= 0 && cursor_row < MAX_ROW); // Error handling: cursor_row dalam batas
    assert(cursor_col >= 0 && cursor_col <= MAX_COL); // Error handling: cursor_col dalam batas
    int len = getLineLength(cursor_row);

    if (cursor_col < len) {
        cursor_col++;
    } else if (cursor_row < total_lines - 1) {
        cursor_row++;
        cursor_col = 0;
    }
    adjust_viewport();
}

// Gerakan cursor ke atas
void move_up() {
    if (cursor_row < 0 || cursor_row >= MAX_ROW) {
        printf("Error: cursor_row (%d) di luar batas [0, %d).\n", cursor_row, MAX_ROW);
        return;
    }
    if (cursor_col < 0 || cursor_col > MAX_COL) {
        printf("Error: cursor_col (%d) di luar batas [0, %d].\n", cursor_col, MAX_COL);
        return;
    }
    assert(cursor_row >= 0 && cursor_row < MAX_ROW); // Error handling: cursor_row dalam batas
    assert(cursor_col >= 0 && cursor_col <= MAX_COL); // Error handling: cursor_col dalam batas
    if (cursor_row > 0) {
        cursor_row--;
        int len = getLineLength(cursor_row);
        if (cursor_col > len) cursor_col = len;
    }
    adjust_viewport();
}

// Gerakan cursor ke bawah
void move_down() {
    if (cursor_row < 0 || cursor_row >= MAX_ROW) {
        printf("Error: cursor_row (%d) di luar batas [0, %d).\n", cursor_row, MAX_ROW);
        return;
    }
    if (cursor_col < 0 || cursor_col > MAX_COL) {
        printf("Error: cursor_col (%d) di luar batas [0, %d].\n", cursor_col, MAX_COL);
        return;
    }
    assert(cursor_row >= 0 && cursor_row < MAX_ROW); // Error handling: cursor_row dalam batas
    assert(cursor_col >= 0 && cursor_col <= MAX_COL); // Error handling: cursor_col dalam batas
    if (cursor_row < total_lines - 1) {
        cursor_row++;
        int len = getLineLength(cursor_row);
        if (cursor_col > len) cursor_col = len;
    }
    adjust_viewport();
}

// Menyesuaikan viewport berdasarkan posisi cursor
void adjust_viewport() {
    if (cursor_row < 0 || cursor_row >= MAX_ROW) {
        printf("Error: cursor_row (%d) di luar batas [0, %d).\n", cursor_row, MAX_ROW);
        return;
    }
    assert(cursor_row >= 0 && cursor_row < MAX_ROW); // Error handling: cursor_row dalam batas
    if (cursor_row < view_row_offset) {
        view_row_offset = cursor_row;
    } 
    else if (cursor_row >= view_row_offset + SCREEN_HEIGHT) {
        view_row_offset = cursor_row - SCREEN_HEIGHT + 1;
    }
}