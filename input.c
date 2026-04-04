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

// Menangani aksi membuka file
void handleOpenAction() {
    char filename[100];
    printf("\n[OPEN] Masukkan nama file: ");
    if (fgets(filename, sizeof(filename), stdin) != NULL) {
        filename[strcspn(filename, "\n")] = 0; // Bersihkan newline
        openFile(filename); // Muat file ke RAM
        handleEditInput(filename); // Masuk mode edit
    }
}

// Menangani aksi membuat file baru
void handleNewFileAction() {
    clearBuffer(); // Kosongkan buffer
    initCursor();  // Reset kursor
    strcpy(current_filename, "Untitled");
    is_modified = 0;
    handleEditInput(""); // Buka editor kosong
}

// Menangani aksi keluar program
void handleExitAction() {
    printf("\n[QUIT] Keluar dari Saw<git>? (y/n): ");
    int confirm = _getch();
    if (confirm == 'y' || confirm == 'Y') {
        clearRecovery(); // Hapus file temporary
        exit(0);
    }
}


void handleEditInput(char *filename) {
    if (total_lines == 0) total_lines = 1;
    extern char current_filename[256];
    strcpy(current_filename, filename);

    while (1) {
        renderScreen(text_buffer, total_lines);
        int c = _getch();

        // Navigasi & Keluar
        if (c == 27) {
        	break; // ESC: Back to Menu	
		}
        else if (c == 15) { // Ctrl+O: Open
            printf("\n[WARNING] Mau membuka file lain? Data saat ini akan hilang (y/n): ");
            if (_getch() == 'y') 
			{
				handleOpenAction();
			}
        }
        else if (c == 14) { // Ctrl+N: New
            printf("\n[WARNING] Mau membuat file baru? Data saat ini akan hilang (y/n): ");
            if (_getch() == 'y') {
            	handleNewFileAction();
			}
        }
        else if (c == 17) {
			handleExitAction(); // Ctrl+Q: Quit
   		 }
        else if (c == 224) { // Arrow Keys
            c = _getch();
            if (c == 72) move_up();
            else if (c == 80) move_down();
            else if (c == 75) move_left();
            else if (c == 77) move_right();
        }

        // Shortcut Info & Help
        else if (c == 7) { // Ctrl+G: Help
            renderHelpScreen();
            _getch();
        }
        else if (c == 9) { // Ctrl+I: Info
            renderInfoScreen();
            _getch();
        }

        // Manajemen File
        else if (c == 19) { // Ctrl+S: Save
            if (strlen(current_filename) > 0 && strcmp(current_filename, "Untitled") != 0) {
                saveToFile(current_filename);
            } else {
                printf("\n[SAVE AS] Masukkan nama file baru: ");
                if (fgets(current_filename, sizeof(current_filename), stdin) != NULL) {
                    current_filename[strcspn(current_filename, "\n")] = 0;
                    if (strlen(current_filename) > 0) saveToFile(current_filename);
                }
            }
            printf("\nTekan sembarang tombol...");
            _getch();
        }

        // Manipulasi Teks (Trigger Recovery)
        else if (c == 8) { // Backspace
            delete_char();
            is_modified = 1;
            writeRecovery();
        } 
        else if (c == 13) { // Enter
            insert_newline(); 
            is_modified = 1;
            writeRecovery(); 
        } 
        else if (c >= 32 && c <= 126) { // Char
            insert_char((char)c); 
            is_modified = 1;
            writeRecovery(); 
        }
    }
}

// Fungsi Menu ditaruh di bawah
void handleMenuInput() {
    int c = _getch(); 

    if (c == '1' || c == 15) 
	{
		handleOpenAction();
	}
    else if (c == '2' || c == 14) {
    	handleNewFileAction();
	} 
    else if (c == '3' || c == 9) { 
        renderInfoScreen();
        _getch();
    }
    else if (c == '4' || c == 7) { 
        renderHelpScreen();
        _getch();
    }
    else if (c == '5' || c == 17) 
	{
		handleExitAction();
	}
}
