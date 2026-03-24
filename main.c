#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <stdbool.h>
#include "recovery.h"
#include "render.h"

#define MAX_TEXT 1000

// Global variable untuk buffer teks
char text[MAX_TEXT] = "";

// Handler untuk interupsi (Ctrl+C atau kill)
void handle_signal(int sig) {
    printf("\n[!] Program diinterupsi. Menyimpan recovery...\n");
    writeRecovery(text);
    printf("[!] Recovery tersimpan. Program keluar.\n");
    fflush(stdout);
    exit(1);
}

int main() {
    char command[50];
    char input[100];
    bool running = true;

    // Pasang "jaring pengaman" signal dari Branch A
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    // Array Dummy untuk fitur render dari Branch B
    char dummyFile[10][100] = {
        "Saw<git> Anggota",
        "1. Putra Surya Pratama",
        "2. R. Nesya Raham Velda",
        "3. Tania Dwi Pangesti"
    };

    // Cek data recovery saat startup
    checkRecovery(text);

    system("cls"); // Bersihkan layar saat mulai
    printf("===== Saw<git> | Text Editor Gabungan =====\n");
    if (strlen(text) > 0) {
        printf("[!] Berhasil memulihkan data dari sesi sebelumnya.\n");
    }

    while (running) {
        printf("\nKetik: ./i (info), ./file (render), ./edit (ketik), ./cls (bersih), ./q (keluar)\n");
        printf("sawgit> ");
        
        // Menggunakan scanf untuk perintah menu
        scanf("%s", command);
        getchar(); // Membersihkan newline dari buffer stdin

        if (strcmp(command, "./i") == 0) {
            printf("\n[INFO] Text Editor Console - Tim Saw<git>.\n");
            printf("Fitur: Autosave, Signal Recovery, & Renderer.\n");
        } 
        else if (strcmp(command, "./file") == 0) {
            renderScreen(dummyFile, 4); 
            printf("\n(Tekan Enter untuk kembali)\n");
            getchar();
        }
        else if (strcmp(command, "./edit") == 0) {
            printf("\n--- MODE EDIT (Ketik 'exit' untuk simpan & kembali, Ctrl+C untuk crash) ---\n");
            printf("Isi saat ini:\n%s\n", text);
            
            while (1) {
                printf("> ");
                fgets(input, sizeof(input), stdin);
                input[strcspn(input, "\n")] = 0;

                if (strcmp(input, "exit") == 0) {
                    printf("Kembali ke menu utama.\n");
                    break;
                }

                if (strlen(input) == 0) continue;

                // Cek overflow buffer
                if (strlen(text) + strlen(input) + 2 < MAX_TEXT) {
                    strcat(text, input);
                    strcat(text, "\n");
                    writeRecovery(text); // Autosave setiap baris
                    printf("[autosave] Tersimpan.\n");
                } else {
                    printf("[!] Buffer penuh!\n");
                    break;
                }
            }
        }
        else if (strcmp(command, "./cls") == 0) {
            system("cls");
        }
        else if (strcmp(command, "./q") == 0) {
            // Keluar normal: hapus recovery karena data dianggap sudah selesai
            clearRecovery();
            printf("Keluar dari program... Sampai jumpa!\n");
            running = false;
        }
        else {
            printf("[ERROR] Perintah '%s' tidak dikenal.\n", command);
        }
    }

    return 0;
}