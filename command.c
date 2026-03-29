#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "command.h"
#include "input.h"    
#include "file_io.h"  
#include "buffer.h"   
#include "cursor.h"   

void executeCommand(char *command) {
    // Menampilkan informasi program
        if (strcmp(command, "./i") == 0) {
            printf("\n[INFO] Text Editor Console - Tim Saw<git>.\n");
            printf("Fitur: Buffer 2D Array, Autosave, Signal Recovery, & Renderer.\n");
        } 
        // Menampilkan isi buffer ke layar
        else if (strcmp(command, "./file") == 0) {
            renderScreen(text_buffer, total_lines); 
            printf("\n(Tekan Enter untuk kembali)\n");
            getchar();
        }
        // Masuk mode edit
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
        // Membuka file dari disk ke buffer
        else if (strcmp(command, "./open") == 0) {
            char filename[100];

            printf("Masukkan nama file yang ingin dibuka: ");
            fgets(filename, sizeof(filename), stdin);
            filename[strcspn(filename, "\n")] = 0;

            if (strlen(filename) == 0) {
                printf("[ERROR] Nama file tidak boleh kosong!\n");
            } else {
                openFile(filename);
                // Tampilkan isi file setelah berhasil dibuka
                if (total_lines > 0) {
                    renderScreen(text_buffer, total_lines);
                }
            }
        }
        // Menyimpan isi buffer ke file
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
        // Membersihkan layar terminal
        else if (strcmp(command, "./cls") == 0) 
		{
            system("cls");
        }
        else if (strcmp(command, "./q") == 0) 
		{
            clearRecovery();
            printf("Anda telah keluar dari program\n");
            exit(0);
        }
        else {
            printf("[ERROR] Perintah '%s' tidak dikenal.\n", command);
        }
}
