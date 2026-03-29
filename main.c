#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>
#include "recovery.h"
#include "render.h"
#include "buffer.h"
#include "cursor.h"
#include "file_io.h"
#include "input.h"
#include "command.h"

// Menangani interupsi (Ctrl+C atau kill) dengan menyimpan data recovery lebih dulu.
void handle_signal(int sig) {
    printf("\n[!] Program diinterupsi. Menyimpan recovery...\n");
    writeRecovery();
    printf("[!] Recovery tersimpan. Program keluar.\n");
    fflush(stdout);
    exit(1);
}

int main() {
    char command[50];
    bool running = true;

    // Pasang handler signal agar data disimpan ketika program diinterupsi.
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    // Inisialisasi buffer teks saat aplikasi mulai.
    initBuffer();

    // Cek dan muat data recovery jika ada.
    checkRecovery();

    system("cls"); // Bersihkan layar saat mulai
    printf("===== Saw<git> | Text Editor Gabungan =====\n");
    if (total_lines > 0) 
	{
        printf("[!] Berhasil memulihkan data dari sesi sebelumnya.\n");
    }

    while (1) 
	{
        printf("\nKetik: ./i (info), ./file (render), ./edit (ketik), ./open (buka), ./save (simpan), ./cls (bersih), ./q (keluar)\n");
        printf("sawgit> ");
        
        scanf("%s", command);
        getchar(); // Membersihkan newline dari buffer stdin
        executeCommand(command);
    }

    return 0;
}
