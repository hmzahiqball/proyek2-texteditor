#include <stdio.h>
#include <string.h>
#include "recovery.h"
#include "buffer.h"

#define RECOVERY_FILE "recovery.tmp"

// checkRecovery - Mengecek apakah ada file recovery saat startup.
// Jika ada, isi file dimuat ke dalam buffer text.
void checkRecovery() {
    FILE *fp = fopen(RECOVERY_FILE, "r");
    if (fp == NULL) {
        printf("Recovery tidak ditemukan.\n");
        return;
    }

    char line[MAX_COL];
    clearBuffer(); // Pastikan buffer bersih sebelum meload dari file
    
    // Load ke Array 2D
    while (fgets(line, sizeof(line), fp) != NULL) {
        // Hapus karakter newline bawaan fgets agar rapi di array kita
        line[strcspn(line, "\n")] = 0; 
        appendLine(line);
    }

    fclose(fp);

    if (total_lines > 0) {
        printf("Recovery ditemukan, %d baris data dimuat.\n", total_lines);
    } else {
        printf("Recovery file kosong.\n");
    }
}

void writeRecovery() {
    FILE *fp = fopen(RECOVERY_FILE, "w");
    if (fp == NULL) {
        printf("Gagal menulis recovery.\n");
        return;
    }

    // Tulis isi Array 2D kembali ke file session.tmp
    int i;
    for (i = 0; i < total_lines; i++) {
        fprintf(fp, "%s\n", text_buffer[i]);
    }
    
    fclose(fp);
}

void clearRecovery() {
    remove(RECOVERY_FILE);
}