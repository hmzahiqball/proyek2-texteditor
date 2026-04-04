#include <stdio.h>
#include <string.h>
#include <conio.h>
#include "file_io.h"
#include "buffer.h"
#include "cursor.h"
#include "recovery.h"

// Deklarasi variabel global untuk status modifikasi dan nama file saat ini
// Variabel ini akan dipinjam (extern) di render.c
int is_modified = 0; // 0 = tidak ada perubahan, 1 = ada perubahan
char current_filename[256] = "Untitled"; // Nama file default saat buat baru

// Fungsi untuk menyimpan isi buffer ke dalam file di disk
void saveToFile(const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("\n[ERROR] Gagal menyimpan ke %s.\n", filename);
        return;
    }

    // Menulis baris demi baris dari buffer ke file
    int i;
    for (i = 0; i < total_lines; i++) {
        fprintf(fp, "%s\n", text_buffer[i]);
    }

    fclose(fp);
    
    // Update status setelah berhasil simpan
    is_modified = 0; 
    strcpy(current_filename, filename);
    clearRecovery(); // Hapus .tmp karena data sudah aman di .txt
    // Begitu user ketik lagi, writeRecovery() otomatis buat .tmp baru
    printf("\n[INFO] File berhasil disimpan ke %s\n", filename);
}

// Fungsi untuk membuka file dari disk dan memuatnya ke buffer RAM
void openFile(const char *filename) {
    FILE *fp = fopen(filename, "r");
    
    if (fp == NULL) {
        printf("\n[ERROR] File '%s' tidak ditemukan atau tidak bisa dibuka.\n", filename);
        return;
    }

    // 1. Bersihkan buffer lama sebelum memuat data baru
    clearBuffer();
    
    // 2. Simpan nama file yang sedang dibuka
    strcpy(current_filename, filename); 

    // 3. Baca isi file baris demi baris
    char line[MAX_COL];
    while (fgets(line, sizeof(line), fp) != NULL) {
        // Bug Fix: Hapus karakter newline agar tampilan editor tidak berantakan
        line[strcspn(line, "\r\n")] = 0;
        appendLine(line);
    }

    fclose(fp);

    // 4. Update status (Baru buka = belum ada perubahan)
    is_modified = 0; 

    // 5. Tambahan : Pindahkan kursor ke akhir dokumen
    set_cursor_to_end();
    
    printf("\n[INFO] %s berhasil dibuka. %d baris dimuat.\n", filename, total_lines);
    printf("Tekan sembarang tombol untuk lanjut...");
    getch();
}