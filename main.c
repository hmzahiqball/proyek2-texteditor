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

#define MAX_TEXT 1000

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
    if (total_lines > 0) {
        printf("[!] Berhasil memulihkan data dari sesi sebelumnya.\n");
    }

    while (running) {
        printf("\nKetik: ./i (info), ./file (render), ./edit (ketik), ./save (simpan), ./cls (bersih), ./q (keluar)\n");
        printf("sawgit> ");
        
        scanf("%s", command);
        getchar(); // Membersihkan newline dari buffer stdin

        if (strcmp(command, "./i") == 0) {
            printf("\n[INFO] Text Editor Console - Tim Saw<git>.\n");
            printf("Fitur: Buffer 2D Array, Autosave, Signal Recovery, & Renderer.\n");
        } 
        else if (strcmp(command, "./file") == 0) {
            renderScreen(text_buffer, total_lines); 
            printf("\n(Tekan Enter untuk kembali)\n");
            getchar();
        }
        else if (strcmp(command, "./edit") == 0) {
            printf("\n--- MODE EDIT ---\n");
            printf("Petunjuk: Ketik karakter untuk insert, Enter untuk baris baru.\n");
            printf("Akhiri sesi edit dengan mengetik 'Esc' lalu tekan Enter.\n");
            printf("----------------------------------------------------------\n");

            renderScreen(text_buffer, total_lines);
            set_cursor_to_end();
            handleEditInput();
            getchar(); 
            printf("\n[!] Keluar dari Mode Edit.\n");
        }

        else if (strcmp(command, "./save") == 0) {
            char filename[100];

            while (1) {
                printf("Masukkan nama file (contoh: output.txt): ");
                fgets(filename, sizeof(filename), stdin);

                filename[strcspn(filename, "\n")] = 0;

                if (strlen(filename) == 0) {
                    printf("[ERROR] Nama file tidak boleh kosong! Silakan coba lagi.\n");
                } else {
                    break; // keluar dari loop kalau valid
                }
            }

            saveToFile(filename);
        }

        else if (strcmp(command, "./cls") == 0) {
            system("cls");
        }
        else if (strcmp(command, "./q") == 0) {
            printf("Keluar dari program... Sampai jumpa!\n");
            running = false;
        }
        else {
            printf("[ERROR] Perintah '%s' tidak dikenal.\n", command);
        }
    }

    return 0;
}
