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

    while (1) 
	{
        renderScreen(text_buffer, total_lines);
        int c = _getch();
        
        // ESC: Kembali ke Menu Utama
        if (c == 27) { 
            break; 
        }

        // Ctrl + Q: Keluar Program Sepenuhnya
        else if (c == 17) { 
            printf("\n[QUIT] Keluar dari Saw<git>? (y/n): ");
            if (_getch() == 'y') {
                clearRecovery(); // Hapus file tmp saat exit normal
                exit(0); 
            }
        }
        // Ctrl+S
		else if (c == 19) { 
            if (strlen(current_file) > 0) 
			{
                saveToFile(current_file);
                printf("\n[OK] Tersimpan ke %s", current_file);
            } else {
                printf("\n[SAVE AS] Masukkan nama file baru: ");
                if (fgets(current_file, sizeof(current_file), stdin) != NULL) 
				{
                    current_file[strcspn(current_file, "\n")] = 0;
                    if (strlen(current_file) > 0) saveToFile(current_file);
                }
            }
            printf("\nTekan sembarang tombol...");
            _getch();
        }
        // Ctrl+G
		else if (c == 7) { 
            renderHelpScreen();
            _getch();
        }
		// Ctrl+Q
        else if (c == 17) 
		{ 
            printf("\n[QUIT] Keluar dari Saw<git>? (y/n): ");
            if (_getch() == 'y') 
			{
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
        else if (c == 8) { 
            delete_char(); 
            writeRecovery(); 
        } 
        else if (c == 13) { 
            insert_newline(); 
            writeRecovery(); 
        } 
        else if (c >= 32 && c <= 126) { 
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
