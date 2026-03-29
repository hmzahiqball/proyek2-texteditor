#include <stdio.h>
#include <string.h>
#include "recovery.h"
#include "buffer.h"
#include "cursor.h"

#define RECOVERY_FILE "recovery.tmp"

// Memuat isi file recovery.tmp ke buffer saat startup jika tersedia.
void checkRecovery() {
    FILE *fp = fopen(RECOVERY_FILE, "r");
    if (fp == NULL) {
        printf("Recovery tidak ditemukan.\n");
        return;
    }

    char line[MAX_COL];
    clearBuffer(); // Kosongkan buffer sebelum memuat kembali dari file

    while (fgets(line, sizeof(line), fp) != NULL) {
        line[strcspn(line, "\n")] = 0; 
        appendLine(line);
    }

    fclose(fp);

    set_cursor_to_end();

    if (total_lines > 0) {
        printf("Recovery ditemukan, %d baris data dimuat.\n", total_lines);
    } else {
        printf("Recovery file kosong.\n");
    }
}

// Menyimpan isi buffer saat ini ke file recovery.tmp untuk pemulihan sesi.
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

// Menghapus file recovery saat exit normal
void clearRecovery() {
    remove(RECOVERY_FILE);
}