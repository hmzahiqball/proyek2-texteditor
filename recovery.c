#include <stdio.h>
#include <string.h>
#include <conio.h>
#include "recovery.h"
#include "buffer.h"
#include "cursor.h"

#define RECOVERY_FILE "recovery.tmp"
extern char current_filename[256];
extern int is_modified; // Status modifikasi buffer, dipinjam dari file_io.c

// Memuat isi file recovery.tmp ke buffer saat startup jika tersedia.
int checkRecovery() {
    FILE *fp = fopen(RECOVERY_FILE, "r");
    if (fp == NULL) {
        printf("Recovery tidak ditemukan.\n");
        writeRecovery();  //agar file recovery dibuat untuk sesi berikutnya
        return 0; // Tidak ada recovery, lanjutkan normal
    }

    char line[MAX_COL];
    clearBuffer();

    // 1. Baca baris pertama untuk mencari tahu nama file aslinya
    if (fgets(line, sizeof(line), fp) != NULL) {
        if (strncmp(line, "FILENAME:", 9) == 0) {
            line[strcspn(line, "\r\n")] = 0; // Bersihkan newline
            strcpy(current_filename, line + 9); // Ambil nama setelah "FILENAME:"
        } else {
            // Jika baris pertama bukan FILENAME (file lama), 
            // anggap sebagai teks biasa dan masukkan ke buffer
            line[strcspn(line, "\r\n")] = 0;
            appendLine(line);
        }
    }

    // 2. Baca sisa baris lainnya sebagai isi teks
    while (fgets(line, sizeof(line), fp) != NULL) {
        line[strcspn(line, "\r\n")] = 0;
        appendLine(line);
    }

    fclose(fp);
    set_cursor_to_end();
    is_modified = 1 ; // Tandai buffer sebagai sudah dimodifikasi agar user tahu ada data yang dimuat

    if (total_lines > 0) {
        printf("[!] Recovery ditemukan, %d baris dimuat.\n", total_lines); 
        printf("Tekan sembarang tombol untuk lanjut...");
        getch();
    } else {
        printf("Recovery file kosong.\n");
    }

    return 1; // Recovery berhasil dimuat
}

// Menyimpan isi buffer saat ini ke file recovery.tmp untuk pemulihan sesi.
void writeRecovery() {
    // PROTEKSI: Jangan tulis kalau buffer kosong (total_lines == 0)
    // Kecuali kalau memang user sengaja mengosongkan semua teks.
    if (total_lines == 0) return; // Tidak perlu buat file recovery kalau tidak ada data untuk disimpan

    FILE *fp = fopen("recovery_new.tmp", "w"); 
    if (fp == NULL) return;
    fprintf(fp, "FILENAME:%s\n", current_filename); // Simpan nama file yang sedang dibuka (jika ada)

    for (int i = 0; i < total_lines; i++) {
        fprintf(fp, "%s\n", text_buffer[i]); // Simpan setiap baris teks ke file recovery
    }
    
    fclose(fp); 

    remove(RECOVERY_FILE); // Hapus file recovery lama
    rename("recovery_new.tmp", RECOVERY_FILE); // Ganti nama file baru ke recovery.tmp
}

// Menghapus file recovery saat exit normal
void clearRecovery() {
    remove(RECOVERY_FILE);
}