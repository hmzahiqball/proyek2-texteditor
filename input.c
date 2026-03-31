#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include "input.h"
#include "buffer.h"
#include "cursor.h"
#include "render.h"
#include "file_io.h"
#include "recovery.h"


void handleMenuInput() {
    int pilihan;
    // Validasi agar jika user input huruf, program tidak looping error
    if (scanf("%d", &pilihan) != 1) {
        while (getchar() != '\n'); 
        return;
    }
    getchar(); // Membersihkan newline dari buffer stdin

    switch (pilihan) {
        case 1: // Open File
            {
                char filename[100];
                printf("Masukkan nama file: ");
                fgets(filename, sizeof(filename), stdin);
                filename[strcspn(filename, "\n")] = 0;
                
                openFile(filename); // Memuat file ke buffer
                handleEditInput();  // Langsung masuk ke mode pengetikan
            }
            break;

        case 2: // Create File
            clearBuffer();  // Reset isi teks
            initCursor();   // Reset posisi kursor ke (0,0)
            handleEditInput();
            break;

        case 3: // Info (Pengganti ./i)
            printf("\n[INFO] Text Editor Console - Tim Saw<git>.\n");
            printf("Fitur: Buffer 2D Array, Autosave, Signal Recovery, & Renderer.\n");
            printf("\nTekan Enter untuk kembali...");
            getchar();
            break;

        case 4: // Help (Pengganti ./help)
            system("cls");
            printf("--- PANDUAN PENGGUNA ---\n");
            printf("Navigasi : Tombol Panah\n");
            printf("Simpan   : (Fitur Ctrl+S segera hadir)\n");
            printf("Kembali  : Tekan ESC saat di mode edit\n");
            printf("\nTekan Enter untuk kembali...");
            getchar();
            break;

        case 5: // Quit (Pengganti ./q)
            clearRecovery(); // Hapus file tmp karena keluar normal
            printf("Keluar dari program. Sampai jumpa!\n");
            exit(0);
            break;

        default:
            printf("\n[!] Pilihan 1-5 saja ya, Tan. wkwk\n");
            _getch(); // Tunggu sebentar agar pesan terbaca
            break;
    }
}


void handleEditInput()
{
	// Loop utama mode edit: baca tombol dan ubah isi buffer.
	
	if (total_lines == 0) 
	{
        total_lines = 1; // Beri ruang satu baris untuk mulai mengetik
    }
    
    
    while (1) 
	{
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

}


