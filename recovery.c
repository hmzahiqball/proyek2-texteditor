#include <stdio.h>
#include <string.h>
#include "recovery.h"

#define RECOVERY_FILE "recovery.tmp"

// checkRecovery - Mengecek apakah ada file recovery saat startup.
// Jika ada, isi file dimuat ke dalam buffer text.
void checkRecovery(char *text) {
    FILE *fp = fopen(RECOVERY_FILE, "r");

    if (fp == NULL) {
        printf("Recovery tidak ditemukan.\n");
        return;
    }

    int ch;
    int i = 0;

    while ((ch = fgetc(fp)) != EOF && i < 999) {
        text[i++] = (char)ch;
    }
    text[i] = '\0';

    fclose(fp);

    if (i > 0) {
        printf("Recovery ditemukan, data dimuat.\n");
    } else {
        printf("Recovery file kosong.\n");
    }
}

// writeRecovery - Menulis isi buffer ke file recovery.
// Dipanggil setiap ada perubahan (autosave) dan saat crash.
void writeRecovery(const char *text) {
    FILE *fp = fopen(RECOVERY_FILE, "w");

    if (fp == NULL) {
        printf("Gagal menulis recovery.\n");
        return;
    }

    fprintf(fp, "%s", text);
    fclose(fp);
}

// clearRecovery - Menghapus file recovery.
// Dipanggil saat program keluar secara normal.
void clearRecovery() {
    remove(RECOVERY_FILE);
}