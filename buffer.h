#ifndef BUFFER_H
#define BUFFER_H

#define MAX_ROW 100
#define MAX_COL 100

// Deklarasi array 2D global sesuai spesifikasi Saw<git>
extern char text_buffer[MAX_ROW][MAX_COL];
extern int total_lines;

// State Kursor
extern int cursor_row;
extern int cursor_col;

// Fungsi untuk inisialisasi buffer kosong
void initBuffer();

// Fungsi dasar CRU (Create, Read, Update): manipulasi langsung elemen array
void appendLine(const char *input);

// Menghapus seluruh isi buffer
void clearBuffer();

// Fungsi untuk memastikan cursor tidak keluar dari batas buffer
void limitCursorBounds();

// Menyisipkan karakter tunggal pada posisi kursor
void insert_char(char c);

// Delete Karakter (Backspace)
void delete_char();

// Insert Newline (Enter)
void insert_newline();

#endif