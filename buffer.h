#ifndef BUFFER_H
#define BUFFER_H

#define MAX_ROW 100
#define MAX_COL 100

// Deklarasi array 2D global sesuai spesifikasi Saw<git>
extern char text_buffer[MAX_ROW][MAX_COL];
extern int total_lines;

// Fungsi untuk inisialisasi buffer kosong
void initBuffer();

// Fungsi dasar CRU (Create, Read, Update): manipulasi langsung elemen array
void appendLine(const char *input);

// Menghapus seluruh isi buffer
void clearBuffer();

#endif