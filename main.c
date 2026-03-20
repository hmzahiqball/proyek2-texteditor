#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "render.h"

int main() {
    char command[50];
    bool running = true;

    // Array Dummy untuk ditampilkan
    char dummyFile[10][100] = {
        "Saw<git> Anggota",
        "1. Putra Surya Pratama",
        "2. R. Nesya Raham Velda",
        "3. Tania Dwi Pangesti"
    };

    while (running) {
        // Tampilan Awal Text Editor
        printf("===== Saw<git> | Text Editor =====\n");
        printf("Ketik ./i untuk informasi, ./file untuk buka file, ./cls untuk membersihkan terminal, ./q untuk keluar\n");
        printf("sawgit> ");
        
        // Mengambil input perintah dari user 
        scanf("%s", command);

        if (strcmp(command, "./i") == 0) {
            printf("\n[INFO] Ini adalah Text Editor Console buatan Tim Saw<git>.\n\n");
        } 
        else if (strcmp(command, "./file") == 0) {
            // Memanggil fungsi Renderer kamu untuk menampilkan array dummy
            renderScreen(dummyFile, 4); 
            printf("\n(Tekan Enter untuk kembali ke menu utama)\n");
            getchar(); getchar(); // Menahan layar
        }
        else if (strcmp(command, "./cls") == 0) {
            system("cls");
        }
        else if (strcmp(command, "./q") == 0) {
            printf("Keluar dari program... Sampai jumpa!\n");
            running = false; // Menghentikan loop -> perintah keluar dari program
        }
        else {
            printf("[ERROR] Perintah '%s' tidak dikenal.\n\n", command);
        }
    }	

    return 0;
}
