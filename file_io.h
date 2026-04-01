#ifndef FILE_IO_H
#define FILE_IO_H

// Deklarasi variabel global untuk status modifikasi dan nama file saat ini, untuk digunakan di file lain seperti render.c
extern int is_modified;
extern char current_filename[256];

// Menyimpan isi buffer ke file .txt
void saveToFile(const char *filename);

// Membuka file .txt dari disk dan memuat isinya ke buffer
void openFile(const char *filename);

#endif