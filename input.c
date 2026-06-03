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

int is_in_editor = 0;

// Helper: Reset notifikasi bawah agar bersih
void clearBottomMessage() 
{
    show_message = 0;
    strcpy(bottom_message, "");
    renderScreen(NULL, total_lines);
}

// Helper: Menandai adanya perubahan dokumen (Autosave triggered)
void markAsModified() 
{
    is_modified = 1;
    show_message = 0; 
    strcpy(bottom_message, ""); 
    writeRecovery(); 
}

// Helper: Konfirmasi pengamanan data y/n 
int askConfirmation(const char *warning_text) 
{
    strcpy(bottom_message, warning_text);
    show_message = 1;
    renderScreen(NULL, total_lines);

    int konfirmasi = _getch();
    if (konfirmasi == 'y' || konfirmasi == 'Y') 
	{
        return 1; // User setuju (Yes)
    }
    clearBottomMessage();
    return 0; // User batal (No)
}

// Menangani aksi membuka file
void handleOpenAction() 
{
    if (is_in_editor == 1) 
	{
        // Jika ada perubahan, minta konfirmasi bypass data
        if (is_modified == 1) 
		{
            if (!askConfirmation("[WARNING] Perubahan belum disimpan! Buka file lain? (y/n): ")) 
			{
				return;
			}
        } 
		else 
		{
            if (!askConfirmation("[OPEN] Buka file lain? (y/n): ")) 
			{
				return;
			}
        }
        
        input_mode = 1; 
        strcpy(bottom_message, "[OPEN] Masukkan nama file yang ingin dibuka: ");
        renderScreen(NULL, total_lines); 
    } 
	else 
	{
        printf("\n[OPEN] Masukkan nama file yang ingin dibuka: "); 
        fflush(stdout);
    }

    char filename[256];
    if (fgets(filename, sizeof(filename), stdin) != NULL) 
	{
        filename[strcspn(filename, "\n")] = 0; 
        input_mode = 0;
        show_message = 0;

        if (strlen(filename) > 0) 
		{
            openFile(filename);
            strncpy(current_filename, filename, sizeof(current_filename) - 1);
            current_filename[sizeof(current_filename) - 1] = '\0';
            handleEditInput(filename);
        }
    }
    input_mode = 0; 
    show_message = 0; 
}

// Menangani aksi membuat file baru
void handleNewFileAction() 
{
    if (is_in_editor == 1) 
	{
        if (is_modified == 1) 
		{
            if (!askConfirmation("[WARNING] Perubahan belum disimpan! Buat file baru? (y/n): ")) 
			{
				return;
			}
        } 
		else 
		{
            if (!askConfirmation("[NEW FILE] Buat file baru? (y/n): ")) 
			{
				return;	
			}
        }
        clearBottomMessage();
    }

    clearBuffer();                    
    appendLine("");                 
    initCursor();                         
    strncpy(current_filename, "Untitled", sizeof(current_filename) - 1);
    current_filename[sizeof(current_filename) - 1] = '\0';
    is_modified = 0;                      
    
    renderScreen(NULL, total_lines);      
    handleEditInput("");                  
}

// Fungsi Save As
void handleSaveAsAction() 
{
    char temp_name[256];
    input_mode = 1; 
    strncpy(bottom_message, "[SAVE AS] Masukkan nama file baru: ", sizeof(bottom_message) - 1);
    bottom_message[sizeof(bottom_message) - 1] = '\0';
    show_message = 1;
    renderScreen(NULL, total_lines); 

    if (fgets(temp_name, sizeof(temp_name), stdin) != NULL) 
    {
        temp_name[strcspn(temp_name, "\n")] = 0;
        
        if (strlen(temp_name) > 0) 
        {
            if (isFileExists(temp_name)) 
            {
                input_mode = 0; // Ubah ke mode konfirmasi getch sejenak
                if (!askConfirmation("[WARNING] File sudah ada! Timpa? (y/n): ")) 
                {
                    strncpy(bottom_message, "[BATAL] Penyimpanan dibatalkan.", sizeof(bottom_message) - 1);
                    bottom_message[sizeof(bottom_message) - 1] = '\0';
                    show_message = 1;
                    input_mode = 0; // FIX: Reset input_mode agar kursor atas tidak terkunci mati
                    return;
                }
            }
            saveToFile(temp_name);
            strncpy(current_filename, temp_name, sizeof(current_filename) - 1);
            current_filename[sizeof(current_filename) - 1] = '\0';
        }
    }
    input_mode = 0;
    show_message = 0;
}
void handleSaveAction() 
{
    if (strcmp(current_filename, "Untitled") == 0 || strlen(current_filename) == 0) 
	{
        handleSaveAsAction();
    } 
	else 
	{
        saveToFile(current_filename);
    }
}

void handleExitAction() 
{
    int confirm = 0;

    if (is_in_editor == 1) 
	{ 
        if (is_modified == 1) 
		{
        	
            confirm = askConfirmation("[WARNING] Perubahan belum disimpan! Tetap keluar? (y/n): ");
        } 
		else 
		{
            confirm = askConfirmation("[QUIT] Keluar dari Saw<git>? (y/n): ");
        }
    } 
	else 
	{
        printf("\n[QUIT] Keluar dari Saw<git>? (y/n): ");
        int c = _getch();
        if (c == 'y' || c == 'Y') confirm = 1;
    }

    if (confirm) 
	{
        clearRecovery();
        printf("\033[H\033[J"); 
        exit(0); 
    }
    clearBottomMessage();
}

void handleEditInput(char *filename) 
{
    is_in_editor = 1; 
    if (total_lines == 0) total_lines = 1; 
    strncpy(current_filename, filename, sizeof(current_filename) - 1);
    current_filename[sizeof(current_filename) - 1] = '\0';

    while (1) 
	{
        renderScreen(NULL, total_lines);
        int c = _getch();
        
        if (c == 27) 
		{ // ESC
            is_in_editor = 0;
            break; 	
        }
        else if (c == 15) 
		{
			handleOpenAction(); 
		}   // Ctrl+O
        else if (c == 14) 
		{
			handleNewFileAction(); // Ctrl+N
		}	
        else if (c == 17) 
		{
			handleExitAction();    // Ctrl+Q
		}	
        else if (c == 19) 
		{
			handleSaveAction(); // Ctrl+S
		}    
        else if (c == 1)  
		{
			handleSaveAsAction();  // Ctrl+A
		}	
        else if (c == 8) 
		{                       // Backspace
            delete_char();
            markAsModified();
        }
        else if (c == 13) 
		{                      // Enter
            insert_newline(); 
            markAsModified(); 
        } 
        else if (c == 224) 
		{                     // Tombol Fungsi (Panah/Delete)
            c = _getch();
            if (c == 72) 
			{
				move_up();
			}
            else if (c == 80) 
			{
				move_down();
			}
            else if (c == 75) 
			{
				move_left();
			}
            else if (c == 77)
			{
				move_right();
			}
            else if (c == 83) 
			{
                delete_forward();
                markAsModified();
            }
        }
        else if (c == 7) 
		{                       // Ctrl+G
            renderHelpScreen();
            _getch();
        }
        else if (c == 9) 
		{                       // Ctrl+I
            renderInfoScreen();
            _getch();
        }
        else if (c >= 32 && c <= 126) 
		{          // Karakter Standar
            insert_char((char)c); 
            markAsModified(); 
        }
    }
}

void handleMenuInput() 
{
    int c = _getch(); 
    if (c == '1' || c == 15) 
	{
		handleOpenAction();
	}
    else if (c == '2' || c == 14) 
	{
    	handleNewFileAction();
	} 
    else if (c == '3' || c == 9) 
	{ 
        renderInfoScreen();
        _getch();
    }
    else if (c == '4' || c == 7) 
	{ 
        renderHelpScreen();
        _getch();
    }
    else if (c == '5' || c == 17) 
	{
		handleExitAction();
	}
}
