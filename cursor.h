#include "buffer.h"

#ifndef CURSOR_H
#define CURSOR_H

// Variabel global untuk posisi cursor
extern int cursor_row;
extern int cursor_col;
extern LineNode *current_line; // pointer ke line saat ini

// Inisialisasi cursor ke posisi awal (0,0)
void initCursor();

// Membatasi posisi cursor agar tetap dalam batas buffer
void limitCursorBounds();

// Mengatur cursor ke akhir dokumen
void set_cursor_to_end();

// Gerakan cursor ke kiri
void move_left();

// Gerakan cursor ke kanan
void move_right();

// Gerakan cursor ke atas
void move_up();

// Gerakan cursor ke bawah
void move_down();

// Menyesuaikan viewport berdasarkan posisi cursor
void adjust_viewport();
void adjust_horizontal_viewport();

#endif