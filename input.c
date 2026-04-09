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

void markAsModified() {
    is_modified = 1;
    writeRecovery(); // Pastikan recovery file selalu update tiap ada ketikan
}

// Menangani aksi membuka file
void handleOpenAction() {
    // 1. LOGIKA SAAT DI DALAM EDITOR (Ctrl+O)
    if (is_in_editor == 1) 
	{
        // Cek apakah ada perubahan yang belum disimpan
        if (is_modified == 1) 
		{
            strcpy(bottom_message, "[WARNING] Perubahan belum disimpan! Buka file lain? (y/n): ");
        } 
		else 
		{
            strcpy(bottom_message, "[OPEN] Buka file lain? (y/n): ");
        }
        
        show_message = 1;
        renderScreen(text_buffer, total_lines);

        // Jika user batal (tidak tekan 'y'), kembali ke editor
        if (_getch() != 'y') {
            show_message = 0;
            return; 
        }
        
        // Jika lanjut, minta nama file di baris pesan bawah
        input_mode = 1; 
        strcpy(bottom_message, "[OPEN] Masukkan nama file: ");
        renderScreen(text_buffer, total_lines);
    } 
    // 2. LOGIKA SAAT DI MENU UTAMA (Pilih angka 1)
    else {
        // Tampilkan instruksi tepat di samping prompt "Sawgit> "
        printf("\n[OPEN] Masukkan nama file: "); 
        fflush(stdout); // Memastikan teks muncul sebelum fgets dijalankan
    }

    // 3. PROSES PENGAMBILAN INPUT (Berlaku untuk Editor & Menu)
    char filename[100];
    if (fgets(filename, sizeof(filename), stdin) != NULL) 
	{
        // Hilangkan karakter newline di akhir input
        filename[strcspn(filename, "\n")] = 0; 
        
        // Reset state visual agar kursor kembali ke posisi normal
        input_mode = 0; 
        show_message = 0;

        // Jika nama file tidak kosong, muat file dan masuk ke editor
        if (strlen(filename) > 0) {
            openFile(filename);
            handleEditInput(filename);
        }
    }
    
    // Pastikan status selalu bersih jika input dibatalkan atau gagal
    input_mode = 0; 
    show_message = 0; 
}

// Menangani aksi membuat file baru
void handleNewFileAction() 
{
    // 1. Cek pengaman jika dipanggil dari editor
    if (is_in_editor == 1) 
	{
        if (is_modified == 1) 
		{
            strcpy(bottom_message, "[WARNING] Perubahan belum disimpan! Buat file baru? (y/n): ");
        } 
		else 
		{
            strcpy(bottom_message, "[NEW FILE] Buat file baru? (y/n): ");
        }
        
        show_message = 1;
        renderScreen(text_buffer, total_lines);

        if (_getch() != 'y') {
            show_message = 0;
            return; // Batal eksekusi
        }
        show_message = 0;
    }

    // 2. Alur pembuatan file baru seperti biasa
    clearBuffer();
    initCursor();
    strcpy(current_filename, "Untitled");
    is_modified = 0;
    handleEditInput(""); // Masuk kembali ke editor
}

void handleSaveAction()
{
	if (strlen(current_filename) > 0 && strcmp(current_filename, "Untitled") != 0) 
	{
        saveToFile(current_filename);

        sprintf(bottom_message, "[INFO] Perubahan berhasil disimpan ke file %s", current_filename);
        show_message = 1;
    } 
	else 
	{
        input_mode = 1;
        // tampilkan SAVE AS di UI
        strcpy(bottom_message, "[SAVE AS] Masukkan nama file baru: ");
        show_message = 1;
        renderScreen(text_buffer, total_lines);

        // input nama file dari user
        fgets(current_filename, sizeof(current_filename), stdin);
        current_filename[strcspn(current_filename, "\n")] = 0;

        input_mode = 0;

        if (strlen(current_filename) > 0) 
		{
            saveToFile(current_filename);

            sprintf(bottom_message, "[INFO] Perubahan berhasil disimpan ke file %s", current_filename);
            show_message = 1;
        }
    }

    // tampilkan message + pause
    strcat(bottom_message, "\nTekan sembarang tombol...");
    renderScreen(text_buffer, total_lines);
    _getch();

    show_message = 0; // reset biar hilang
}

void handleExitAction() 
{
    if (is_in_editor == 1) 
	{ 
        if (is_modified == 1) 
		{
            // Pesan Warning keras karena ada data yang belum aman
            strcpy(bottom_message, "[WARNING] Perubahan belum disimpan! Tetap keluar? (y/n): ");
        } 
		else 
		{
            // Pesan santai karena data sudah aman
            strcpy(bottom_message, "[QUIT] Keluar dari Saw<git>? (y/n): ");
        }
        show_message = 1;
        renderScreen(text_buffer, total_lines);
    } 
	else 
	{
        // Kalau is_in_editor == 0, pasti lagi di menu utama
		printf("\n[QUIT] Keluar dari Saw<git>? (y/n): ");
    }

    int confirm = _getch();
    if (confirm == 'y' || confirm == 'Y') 
	{
        clearRecovery();
        system("cls");
        exit(0); 
    }
    
    // Reset status jika batal
    show_message = 0;
    strcpy(bottom_message, "");
}

void handleEditInput(char *filename) 
{
	is_in_editor = 1;
	
    if (total_lines == 0) total_lines = 1;
    extern char current_filename[256];
    strcpy(current_filename, filename);

    while (1) 
	{
        renderScreen(text_buffer, total_lines);
        int c = _getch();

        // ESC: Back to Menu
        if (c == 27) 
		{
        	is_in_editor = 0;
        	break; 	
		}
		// Ctrl+O: Open
        else if (c == 15) 
		{ 
			handleOpenAction();
        }
        // Ctrl+N: New
        else if (c == 14) 
		{ 
        	handleNewFileAction();
        }
        // Ctrl+Q: Quit
        else if (c == 17) 
		{
			handleExitAction(); 
   		}
   		// Arrow Keys
        else if (c == 224) 
		{ 
            c = _getch();
            if (c == 72) move_up();
            else if (c == 80) move_down();
            else if (c == 75) move_left();
            else if (c == 77) move_right();
        }
        // Shortcut Info & Help
        // Ctrl+G: Help
        else if (c == 7) 
		{ 
            renderHelpScreen();
            _getch();
        }
        else if (c == 9) 
		{ // Ctrl+I: Info
            renderInfoScreen();
            _getch();
        }
        // Manajemen File
        else if (c == 19) 
		{ // Ctrl+S
            handleSaveAction();
        }
        // Manipulasi Teks (Trigger Recovery)
        // Backspace
        else if (c == 8) { 
            delete_char();
            markAsModified();
        } 
        // Delete key (224 + 83)
        else if (c == 224) {
            c = _getch();
            if (c == 83) { // Delete
                delete_char_forward();
                markAsModified();
            }
        }
        // Enter
        else if (c == 13) 
		{ 
            insert_newline(); 
            markAsModified(); 
        } 
        else if (c >= 32 && c <= 126) 
        // Char
		{ 
            insert_char((char)c); 
            markAsModified(); 
        }
    }
}

// Fungsi Menu ditaruh di bawah
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
