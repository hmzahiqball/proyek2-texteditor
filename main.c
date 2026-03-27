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
            // Loop utama mode edit: baca tombol dan ubah isi buffer.
            while (1) {
                renderScreen(text_buffer, total_lines);
                
                int c = _getch();

                if (c == 27) { 
                    system("cls"); // Bersihkan layar editor saat kembali ke menu
                    break;
                } 
                // Di Windows, tombol panah mengirim dua kode; kode pertama adalah 224.
                else if (c == 224) { 
                    c = _getch();
                    if (c == 72) move_up();
                    else if (c == 80) move_down();
                    else if (c == 75) move_left();
                    else if (c == 77) move_right();
                } 
                // ASCII 8 adalah Backspace di lingkungan Windows.
                else if (c == 8) { 
                    delete_char();
                    writeRecovery();
                } 
                // ASCII 13 adalah tombol Enter dari _getch().
                else if (c == 13) { 
                    insert_newline();
                    writeRecovery();
                } 
                // Tombol karakter biasa (huruf, angka, spasi, dll.).
                else { 
                    insert_char((char)c);
                    writeRecovery();
                }
            }
            getchar(); 
            printf("\n[!] Keluar dari Mode Edit.\n");
        }

        else if (strcmp(command, "./save") == 0) {
            char filename[100];
            printf("Masukkan nama file (contoh: output.txt): ");
            scanf("%s", filename);
            getchar(); // Membersihkan newline bawaan scanf
            
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