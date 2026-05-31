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

// Fungsi pembantu untuk menandai adanya perubahan pada dokumen
void markAsModified() {
    is_modified = 1; // Mengubah status file menjadi belum disimpan (unsaved)
    
    //Bersihkan notifikasi lama begitu user mengetik karakter baru
    show_message = 0; 
    strcpy(bottom_message, ""); 
    
    writeRecovery(); // Mencadangkan perubahan ke file sementara (autosave)
}

// Menangani aksi membuka file
void handleOpenAction() {
    // 1. Jika sedang berada di dalam layar editor
    if (is_in_editor == 1) 
    {
        // Cek status modifikasi untuk memberikan peringatan pengamanan data
        if (is_modified == 1) 
        {
            strcpy(bottom_message, "[WARNING] Perubahan belum disimpan! Buka file lain? (y/n): ");
        } 
        else 
        {
            strcpy(bottom_message, "[OPEN] Buka file lain? (y/n): ");
        }
        
        show_message = 1;
        renderScreen(NULL, total_lines);

        // Konfirmasi getch aman dipakai di sini
        int konfirmasi = _getch();
        if (konfirmasi != 'y' && konfirmasi != 'Y') {
            show_message = 0;
            strcpy(bottom_message, "");      // Kosongkan pesan hantu di memori
            renderScreen(NULL, total_lines); // Paksa gambar ulang layar agar terminal bersih seketika!
            return;                          // Kembali ke editor dengan aman tanpa ketikan bocor
        }
    
        input_mode = 1; // FIXED: Aktifkan input mode SEBELUM render agar kursor melompat ke bawah bawah
        strcpy(bottom_message, "[OPEN] Masukkan nama file yang ingin dibuka: ");
        renderScreen(NULL, total_lines); // Render ulang layar agar kursor pindah ke bawah mengawal input user
    } 
    // 2. Jika dipanggil dari Menu Utama (pilihan nomor 1)
    else 
    {
        printf("\n[OPEN] Masukkan nama file yang ingin dibuka: "); 
        fflush(stdout); // Memastikan teks muncul sebelum fgets dijalankan
    }

    // 3. Proses pengambilan string nama file dari user
    char filename[100]; // Buffer sementara untuk menampung input nama file
    if (fgets(filename, sizeof(filename), stdin) != NULL) 
    {
        filename[strcspn(filename, "\n")] = 0; // Menghapus karakter Enter (\n) di akhir input
        
        input_mode = 0;        // Matikan mode input (kursor kembali ke area teks)
        show_message = 0;      // Sembunyikan pesan instruksi

        // Jika nama file tidak kosong, muat file dan masuk ke editor
        if (strlen(filename) > 0) 
        {
            openFile(filename);      // Membuka dan memuat isi file ke buffer
            strncpy(current_filename, filename, sizeof(current_filename) - 1);
            current_filename[sizeof(current_filename) - 1] = '\0';
            handleEditInput(filename); // Pindah ke loop utama editor
        }
    }
    
    // Pastikan status selalu bersih jika input dibatalkan atau gagal
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
            strcpy(bottom_message, "[WARNING] Perubahan belum disimpan! Buat file baru? (y/n): ");
            show_message = 1;
            renderScreen(NULL, total_lines);

            int konfirmasi = _getch(); 
            if (konfirmasi != 'y' && konfirmasi != 'Y') {
                show_message = 0;
                strcpy(bottom_message, "");
                renderScreen(NULL, total_lines); 
                return; 
            }
        } 
        else 
        {
            strcpy(bottom_message, "[NEW FILE] Buat file baru? (y/n): ");
            show_message = 1;
            renderScreen(NULL, total_lines);
            
            int konfirmasi = _getch();
            if (konfirmasi != 'y' && konfirmasi != 'Y') {
                show_message = 0;
                strcpy(bottom_message, "");
                renderScreen(NULL, total_lines);
                return;
            }
        }
        
        show_message = 0;
        strcpy(bottom_message, "");
    }

    // Alur pembuatan file baru
    clearBuffer();                    
    appendLine("");                 
    initCursor();                         
    strncpy(current_filename, "Untitled", sizeof(current_filename) - 1);
    current_filename[sizeof(current_filename) - 1] = '\0';
    is_modified = 0;                      
    
    renderScreen(NULL, total_lines);      
    handleEditInput("");                  
}

// Fungsi Save As: Selalu minta nama dan cek duplikasi
void handleSaveAsAction() {
    char temp_name[256];
    input_mode = 1; // FIXED: Aktifkan input mode SEBELUM render agar kursor melompat ke bawah bawah
    strncpy(bottom_message, "[SAVE AS] Masukkan nama file baru: ", sizeof(bottom_message) - 1);
    bottom_message[sizeof(bottom_message) - 1] = '\0';
    show_message = 1;
    renderScreen(NULL, total_lines); // Kursor otomatis diam manis di sebelah nama file baru

    if (fgets(temp_name, sizeof(temp_name), stdin) != NULL) {
        temp_name[strcspn(temp_name, "\n")] = 0;
        
        if (strlen(temp_name) > 0) {
            if (isFileExists(temp_name)) {
                strncpy(bottom_message, "[WARNING] File sudah ada! Timpa? (y/n): ", sizeof(bottom_message) - 1);
                bottom_message[sizeof(bottom_message) - 1] = '\0';
                input_mode = 0; // Kembalikan ke 0 sejenak karena ini mode konfirmasi getch
                renderScreen(NULL, total_lines);
                if (_getch() != 'y') {
                    strncpy(bottom_message, "[BATAL] Penyimpanan dibatalkan.", sizeof(bottom_message) - 1);
                    bottom_message[sizeof(bottom_message) - 1] = '\0';
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

// Fungsi Save: Update file aktif atau lempar ke Save As jika Untitled
void handleSaveAction() 
{
    if (strcmp(current_filename, "Untitled") == 0 || strlen(current_filename) == 0) {
        handleSaveAsAction();
    } else {
        saveToFile(current_filename);
    }
}

//Menangani penutupan program
void handleExitAction() 
{
    if (is_in_editor == 1) 
	{ 
        if (is_modified == 1) 
		{
            strcpy(bottom_message, "[WARNING] Perubahan belum disimpan! Tetap keluar? (y/n): ");
        } 
		else 
		{
            strcpy(bottom_message, "[QUIT] Keluar dari Saw<git>? (y/n): ");
        }
        show_message = 1;
        renderScreen(NULL, total_lines);
    } 
	else 
	{
		printf("\n[QUIT] Keluar dari Saw<git>? (y/n): ");
    }

    int confirm = _getch();
    if (confirm == 'y' || confirm == 'Y') 
	{
        clearRecovery();
        printf("\033[H\033[J"); // Hapus layar total dengan ANSI sebelum keluar bersih
        exit(0); 
    }
    
    show_message = 0;
    strcpy(bottom_message, "");
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
        
        if (c == 27) // ESC (Kembali ke menu utama)
		{
        	is_in_editor = 0;
        	break; 	
		}
        else if (c == 15) // Ctrl+O (Buka file)
		{ 
			handleOpenAction();
        }
        else if (c == 14) // Ctrl+N (Buat file baru)
		{ 
        	handleNewFileAction();
        }
        else if (c == 17)  // Ctrl+Q (keluar)
		{
			handleExitAction(); 
   		}
        else if (c == 224) // Kode awal untuk tombol fungsi (Panah, Delete, dll)
		{ 
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
            else if (c == 83)  // Delete key
			{
				delete_forward();
				markAsModified();
			}
        }
        else if (c == 7) // Ctrl+G (Help : Informasi Shortcut)
		{ 
            renderHelpScreen();
            _getch();
        }
        else if (c == 9) // Ctrl+I (Info) 
		{ 
            renderInfoScreen();
            _getch();
        }
        else if (c == 19) // Ctrl + S
		{ 
		    handleSaveAction(); 
		}
		else if (c == 1) // Ctrl + A (Save As)
		{
		    handleSaveAsAction(); 
		}     
        else if (c == 8)  // Backspace
		{ 
            delete_char();
            markAsModified();
        }
        else if (c == 13) //Enter
		{ 
            insert_newline(); 
            markAsModified(); 
        } 
        else if (c >= 32 && c <= 126) //Pengetikan karakter standar (huruf, angka, simbol)
		{ 
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
