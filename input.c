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
    int c = _getch(); 

    if (c == 17) { // Ctrl+Q
        printf("\n[QUIT] Keluar dari Saw<git>? (y/n): ");
        int confirm = _getch();
        if (confirm == 'y' || confirm == 'Y') {
            clearRecovery();
            exit(0);
        }
        return;
    }
	
	// Shortcut Ctrl+G di Menu Utama
    if (c == 7) {
        renderHelpScreen(); //
        _getch();
        return;
    }
	
    switch (c - '0') {
        case 1: { // OPEN FILE
            char filename[100];
            printf("\nMasukkan nama file yang ingin dibuka: ");
            if (fgets(filename, sizeof(filename), stdin) != NULL) {
                filename[strcspn(filename, "\n")] = 0;
                openFile(filename);
                handleEditInput(filename);
            }
            break;
        }
        case 2: // CREATE FILE
            clearBuffer();
            initCursor();
            handleEditInput(""); // String kosong menandakan file baru
            break;
        case 3: // Info
            printf("\n[INFO] Saw<git> Text Editor v1.0\n");
            printf("Tania (UI), Putra (Logic), Neysa (IO/Recovery)\n");
            printf("\nTekan sembarang tombol...");
            _getch();
            break;
        case 4: // Help
            system("cls");
            renderHelpScreen(); 
            _getch();
            break;
        case 5: // Quit manual
            clearRecovery();
            exit(0);
            break;
    }
}

    
    
void handleEditInput(char *filename) {
    if (total_lines == 0) total_lines = 1;
    char current_file[100];
    strcpy(current_file, filename);

    while (1) {
        renderScreen(text_buffer, total_lines);
        int c = _getch();

        if (c == 17) { // Ctrl+Q
            printf("\n[QUIT] Keluar dari Saw<git>? (y/n): ");
            if (_getch() == 'y') {
                clearRecovery();
                exit(0); 
            }
        }
        else if (c == 19) { // Ctrl+S
            if (strlen(current_file) > 0) {
                saveToFile(current_file);
                printf("\n[OK] Tersimpan ke %s", current_file);
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
        else if (c == 27) 
		{
			break;	
		} 
		else if (c == 7) 
		{ // Shortcut Ctrl+G
		    renderHelpScreen(); // Panggil dari render.c
		    _getch();           // Tunggu input di input.c
		}
        else if (c == 224) 
		{ // Arrow Keys
            c = _getch();
            if (c == 72) move_up();
            else if (c == 80) move_down();
            else if (c == 75) move_left();
            else if (c == 77) move_right();
        } 
        else if (c == 8) 
		{ 
			delete_char(); 
			writeRecovery(); 
		} 
        else if (c == 13) 
		{ 
			insert_newline(); 
			writeRecovery(); 
		} 
        else if (c >= 32 && c <= 126) 
		{ 
			insert_char((char)c); 
			writeRecovery(); 
		}
    }
}


