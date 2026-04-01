#include <stdio.h>
#include <string.h>
#include <conio.h>
#include "file_io.h"
#include "buffer.h"

//Deklarasi variabel global untuk status modifikasi dan nama file saat ini
int is_modified = 0; 
char current_filename[256] = "Untitled";

// Menyimpan isi buffer ke file .txt (buffer ke disk)
void saveToFile(const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Gagal menyimpan ke %s.\n", filename);
        return;
    }

    for (int i = 0; i < total_lines; i++) {
        fprintf(fp, "%s\n", text_buffer[i]);
    }

    fclose(fp);
    is_modified = 0; // Setelah di-save, status kembali jadi 'Saved'
    strcpy(current_filename, filename);
    
    printf("[INFO] File berhasil disimpan ke %s\n", filename);
}

// Membuka file dari disk dan memuat isinya ke buffer (disk ke buffer)
// Di file_io.c karena terkait langsung dengan operasi file, bukan logika input atau rendering.
void openFile(const char *filename) {
    FILE *fp = fopen(filename, "r");
    
    if (fp == NULL) {
        printf("\n[ERROR] File '%s' tidak ditemukan.\n", filename);
        return;
    }

    clearBuffer();
    strcpy(current_filename, filename); // Catat nama filenya

    char line[MAX_COL];
    while (fgets(line, sizeof(line), fp) != NULL) {
        line[strcspn(line, "\r\n")] = 0;
        appendLine(line);
    }

    fclose(fp);
    is_modified = 0; // Baru dibuka, jadi belum ada perubahan (Saved)
    
    printf("\n[INFO] %s terbuka. Status: buffer terisi [%d Baris]\n", filename, total_lines);
    getch();
}