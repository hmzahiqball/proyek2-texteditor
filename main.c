#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include "recovery.h"

#define MAX_TEXT 1000

// Variabel global untuk menyimpan semua teks yang diketik user
char text[MAX_TEXT] = "";

// Fungsi ini otomatis dipanggil saat program di-Ctrl+C atau di-kill
void handle_signal(int sig) {
    printf("\n[!] Program diinterupsi. Menyimpan recovery...\n");
    writeRecovery(text);
    printf("[!] Recovery tersimpan. Program keluar.\n");
    fflush(stdout);
    exit(1);
}

int main() {
    char input[100];

    // Pasang "jaring pengaman" untuk menangkap Ctrl+C dan kill
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    // Cek apakah ada data yang belum tersimpan dari sesi sebelumnya
    checkRecovery(text);
    printf("Isi text saat ini:\n%s\n", text);
    printf("(Ketik 'exit' untuk keluar normal, Ctrl+C untuk simulasi crash)\n\n");

    while (1) {
        printf("> ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0; // hapus newline di akhir input

        // Abaikan jika user hanya tekan Enter tanpa ketik apapun
        if (strlen(input) == 0) continue;

        // Keluar normal - data selesai, hapus file recovery
        if (strcmp(input, "exit") == 0) {
            clearRecovery();
            printf("Keluar normal, recovery dihapus.\n");
            break;
        }

        // Tambahkan input ke buffer, tapi cek dulu supaya tidak overflow
        if (strlen(text) + strlen(input) + 2 < MAX_TEXT) {
            strcat(text, input);
            strcat(text, "\n");
        } else {
            printf("[!] Buffer penuh!\n");
            continue;
        }

        // Simpan otomatis setiap kali ada perubahan
        writeRecovery(text);
        printf("[autosave] Recovery diperbarui.\n");
    }

    return 0;
}