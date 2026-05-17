#include <stdio.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include "recovery.h"
#include "buffer.h"
#include "cursor.h"

#define RECOVERY_FILE "recovery.tmp"

extern char current_filename[256];
extern int is_modified;

// Tracking waktu terakhir recovery ditulis
static time_t last_recovery_time = 0;

// Memuat isi file recovery.tmp ke buffer saat startup jika tersedia.
int checkRecovery() {
    FILE *fp = fopen(RECOVERY_FILE, "r");
    if (fp == NULL) {
        printf("Recovery tidak ditemukan.\n");
        writeRecovery();
        return 0;
    }

    char line[256];
    clearBuffer();

    // Baca baris pertama untuk cek FILENAME
    if (fgets(line, sizeof(line), fp) != NULL) {
        if (strncmp(line, "FILENAME:", 9) == 0) {
            line[strcspn(line, "\r\n")] = 0;
            strcpy(current_filename, line + 9);
        } else {
            line[strcspn(line, "\r\n")] = 0;
            appendLine(line);
        }
    }

    // Baca sisa baris sebagai isi teks
    while (fgets(line, sizeof(line), fp) != NULL) {
        line[strcspn(line, "\r\n")] = 0;
        appendLine(line);
    }

    fclose(fp);
    set_cursor_to_end();
    is_modified = 1;

    if (total_lines > 0) {
        printf("[!] Recovery ditemukan, %d baris dimuat.\n", total_lines);
        printf("Tekan sembarang tombol untuk lanjut...");
        getch();
    } else {
        printf("Recovery file kosong.\n");
    }

    return 1;
}

// Menyimpan isi buffer ke recovery.tmp per 2 detik
void writeRecovery() {
    // Skip kalau belum 2 detik
    time_t now = time(NULL);
    if (difftime(now, last_recovery_time) < 2.0) {
        return;
    }
    last_recovery_time = now;

    if (total_lines == 0) return;

    FILE *fp = fopen("recovery_new.tmp", "w");
    if (fp == NULL) return;

    fprintf(fp, "FILENAME:%s\n", current_filename);

    // Traverse linked list Putra
    LineNode *current = head;
    while (current != NULL) {
        fprintf(fp, "%s\n", current->line);
        current = current->next;
    }

    fclose(fp);
    remove(RECOVERY_FILE);
    rename("recovery_new.tmp", RECOVERY_FILE);
}

// Menghapus file recovery saat exit normal
void clearRecovery() {
    remove(RECOVERY_FILE);
}