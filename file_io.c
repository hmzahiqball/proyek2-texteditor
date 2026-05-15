#include <stdio.h>
#include <string.h>
#include <conio.h>
#include "file_io.h"
#include "buffer.h"
#include "cursor.h"
#include "recovery.h"
#include "render.h"

// Deklarasi variabel global untuk status modifikasi dan nama file saat ini
// Variabel ini akan dipinjam (extern) di render.c
int is_modified = 0; // 0 = tidak ada perubahan, 1 = ada perubahan
char current_filename[256] = ""; // Nama file default saat buat baru

int isFileExists(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp) {
        fclose(fp);
        return 1; // File sudah ada
    }
    return 0; // File belum ada
}


// Fungsi untuk menyimpan isi buffer ke dalam file di disk
void saveToFile(const char *filename) {
    FILE *fp = fopen(filename, "w");

    if (fp == NULL) {
        sprintf(bottom_message, "[ERROR] Gagal menyimpan ke %s", filename);
        show_message = 1;
        return;
    }
    LineNode *current = head;

    while (current != NULL) {
        fprintf(fp, "%s\n", current->line);
        current = current->next;
    }

    fclose(fp);

    is_modified = 0;
    strcpy(current_filename, filename);

    clearRecovery();

    sprintf(bottom_message, "[INFO] File berhasil disimpan ke %s", filename);
    show_message = 1;
}

// Fungsi untuk membuka file dari disk dan memuatnya ke buffer RAM
void openFile(const char *filename) {
    FILE *fp = fopen(filename, "r");

    if (fp == NULL) {
        sprintf(bottom_message, "[ERROR] File '%s' tidak ditemukan.", filename);
        show_message = 1;
        return;
    }

    // Bersihkan buffer lama
    clearBuffer();

    strcpy(current_filename, filename);

    // Buffer baca (dibesarkan biar aman)
    char line[1024];

    while (fgets(line, sizeof(line), fp) != NULL) {

        // Hapus newline
        line[strcspn(line, "\r\n")] = 0;

        // Masukkan ke linked list
        appendLine(line);
    }

    fclose(fp);

    is_modified = 0;

    // Reset cursor
    initCursor();

    sprintf(bottom_message, "[INFO] File '%s' berhasil dibuka (%d baris)", filename, total_lines);
    show_message = 1;
}
