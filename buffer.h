#ifndef BUFFER_H
#define BUFFER_H

#define MAX_ROW 100
#define MAX_COL 100

// Deklarasi array 2D global sesuai spesifikasi Saw<git>
extern char text_buffer[MAX_ROW][MAX_COL];
extern int total_lines;
extern int line_length[MAX_ROW];

int getLineLength(int row);

char getCharAt(int row, int col);

// Fungsi untuk inisialisasi buffer kosong
void initBuffer();

// Fungsi dasar CRU (Create, Read, Update): manipulasi langsung elemen array
void appendLine(const char *input);

// Menghapus seluruh isi buffer
void clearBuffer();

// Menyisipkan karakter tunggal pada posisi kursor
void insert_char(char c);

// Delete Karakter (Backspace)
void delete_char();

// Insert Newline (Enter)
void insert_newline();

int getLineLength(int row);

#endif