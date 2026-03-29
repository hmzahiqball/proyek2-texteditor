#include <stdio.h>
#include <string.h>
#include "file_io.h"
#include "buffer.h"

// Menyimpan isi buffer ke file .txt (buffer ke disk)
void saveToFile(const char *filename) {
    FILE *fp = fopen(filename, "w");

    if (fp == NULL) {
        printf("Gagal menyimpan ke %s.\n", filename);
        return;
    }

    int i;
    for (i = 0; i < total_lines; i++) {
        fprintf(fp, "%s\n", text_buffer[i]);
    }

    fclose(fp);
    printf("[INFO] File berhasil disimpan ke %s\n", filename);
}

// Membuka file dari disk dan memuat isinya ke buffer (disk ke buffer)
void openFile(const char *filename) {
    FILE *fp = fopen(filename, "r");
    
    if (fp == NULL) {
        printf("[ERROR] File '%s' tidak ditemukan.\n", filename);
        return;
    }

    // Kosongkan buffer dulu sebelum load file baru
    clearBuffer();

    char line[MAX_COL];
    while (fgets(line, sizeof(line), fp) != NULL) {
        line[strcspn(line, "\n")] = 0; // hapus \n
        appendLine(line);              // masukkan ke buffer Putra
    }

    fclose(fp);
    printf("[INFO] File '%s' berhasil dibuka. %d baris dimuat.\n", filename, total_lines);
}