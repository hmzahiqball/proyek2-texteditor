#ifndef FILE_IO_H
#define FILE_IO_H

// Menyimpan isi buffer ke file .txt
void saveToFile(const char *filename);

// Membuka file .txt dari disk dan memuat isinya ke buffer
void openFile(const char *filename);

#endif