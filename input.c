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

// Definisi fungsi Editor ditaruh di atas agar Menu mengenalnya
void handleEditInput(char *filename) {
    if (total_lines == 0) total_lines = 1;
    char current_file[100];
    strcpy(current_file, filename);

    while (1) {
        renderScreen(text_buffer, total_lines);
        int c = _getch();

        // --- 1. KELOMPOK NAVIGASI & KELUAR (TIDAK RUBAH DATA) ---
        // --- 1. KELOMPOK NAVIGASI & KELUAR (TIDAK RUBAH DATA) ---
		if (c == 15) { // Ctrl+O (Langsung Buka File Baru)
		    char next_file[100];
		    printf("\n[OPEN] Buka file lain? Data saat ini akan hilang (y/n): ");
		    if (_getch() == 'y') {
		        printf("\nMasukkan nama file yang ingin dibuka: ");
		        if (fgets(next_file, sizeof(next_file), stdin) != NULL) {
		            next_file[strcspn(next_file, "\n")] = 0; // Bersihkan newline
		            
		            // Proses memuat file baru
		            openFile(next_file); 
		            strcpy(current_file, next_file); // Update nama file aktif
		            
		            // Recovery tetap aman karena openFile biasanya memicu reset kursor
		        }
		    }
		}
		else if (c == 27) { // ESC (Hanya ini yang sekarang kembali ke menu)
		    break; 
		}
        else if (c == 17) { // Ctrl+Q (Quit)
            printf("\n[QUIT] Keluar dari Saw<git>? (y/n): ");
            if (_getch() == 'y') {
                clearRecovery();
                exit(0); 
            }
        }
        else if (c == 224) { // Arrow Keys
            c = _getch();
            if (c == 72) move_up();
            else if (c == 80) move_down();
            else if (c == 75) move_left();
            else if (c == 77) move_right();
        }

        // --- 2. KELOMPOK SHORTCUT INFORMASI (TIDAK RUBAH DATA) ---
        else if (c == 7) { // Ctrl+G (Help)
            renderHelpScreen();
            _getch();
        }
        else if (c == 9) { // Ctrl+I (Info)
            printf("\n[INFO] Saw<git> Text Editor v1.0\n");
            printf("Tania (UI), Putra (Logic), Neysa (IO/Recovery)\n");
            printf("\nTekan sembarang tombol...");
            _getch();
        }

        // --- 3. KELOMPOK MANAJEMEN FILE (TIDAK RUBAH DATA) ---
        else if (c == 19) { // Ctrl+S (Save)
            if (strlen(current_file) > 0) {
                saveToFile(current_file);
            } else {
                printf("\n[SAVE AS] Masukkan nama file baru: ");
                if (fgets(current_file, sizeof(current_file), stdin) != NULL) {
                    current_file[strcspn(current_file, "\n")] = 0;
                    if (strlen(current_file) > 0) saveToFile(current_file);
                }
            }
            printf("\nTekan sembarang tombol...");
            _getch();
        }
        else if (c == 14) { // Ctrl+N (New File/Reset Buffer)
            printf("\n[NEW] Buat file baru? Data belum tersimpan akan hilang (y/n): ");
            if (_getch() == 'y') {
                clearBuffer();
                initCursor();
                strcpy(current_file, "");
                total_lines = 1;
            }
        }

        // --- 4. KELOMPOK INPUT TEKS (WAJIB TRIGGER RECOVERY) ---
        else if (c == 8) { // Backspace
            delete_char(); 
            writeRecovery(); 
        } 
        else if (c == 13) { // Enter
            insert_newline(); 
            writeRecovery(); 
        } 
        else if (c >= 32 && c <= 126) { // Karakter cetak
            insert_char((char)c); 
            writeRecovery(); 
        }
    }
}

// Fungsi Menu ditaruh di bawah
void handleMenuInput() {
    int c = _getch(); 

    // 1. OPEN FILE (Angka 1 atau Ctrl + O)
    if (c == '1' || c == 15) { 
        char filename[100];
        printf("\n[OPEN] Masukkan nama file: ");
        if (fgets(filename, sizeof(filename), stdin) != NULL) {
            filename[strcspn(filename, "\n")] = 0; // Bersihkan sisa Enter
            openFile(filename); // Muat file ke RAM
            handleEditInput(filename); // Masuk ke mode edit
        }
    }

    // 2. CREATE NEW FILE (Angka 2 atau Ctrl + N)
    else if (c == '2' || c == 14) { 
        clearBuffer(); // Kosongkan buffer teks
        initCursor();  // Reset posisi kursor ke 0,0
        handleEditInput(""); // Buka editor dengan nama file kosong
    }

    // 3. INFO (Angka 3)
    else if (c == '3' || c == 9) { 
        printf("\n[INFO] Saw<git> Text Editor v1.0\n");
        printf("Tania (UI), Putra (Logic), Neysa (IO/Recovery)\n");
        printf("\nTekan sembarang tombol untuk kembali...");
        _getch();
    }

    // 4. HELP (Angka 4 atau Ctrl + G)
    else if (c == '4' || c == 7) { 
        renderHelpScreen(); // Tampilkan layar panduan
        _getch();
    }

    // 5. QUIT (Angka 5 atau Ctrl + Q)
    else if (c == '5' || c == 17) { 
        printf("\n[QUIT] Keluar dari Saw<git>? (y/n): ");
        int confirm = _getch();
        if (confirm == 'y' || confirm == 'Y') {
            clearRecovery(); // Hapus file temporary agar folder bersih
            exit(0);
        }
    }
}
