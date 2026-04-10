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

// Fungsi pembantu untuk menandai adanya perubahan pada dokumen
void markAsModified() {
    is_modified = 1; // Mengubah status file menjadi belum disimpan (unsaved)
    writeRecovery(); // Mencadangkan perubahan ke file sementara (autosave)
}

// Menangani aksi membuka file
void handleOpenAction() {
    // 1. Jika sedang berada di dalam layar editor (is_in_editor == 1
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
        renderScreen(text_buffer, total_lines);

        // Jika user batal (tidak tekan 'y'), kembali ke editor
        if (_getch() != 'y') {
            show_message = 0;
            return; 
        }
    
        input_mode = 1; // Aktifkan mode input kursor untuk mengetik nama file
        strcpy(bottom_message, "[OPEN] Masukkan nama file: ");
        renderScreen(text_buffer, total_lines); // Render ulang layar agar kursor pindah ke bawah
    } 
    // 2. Jika dipanggil dari Menu Utama (pilihan nomor 1)
    else 
	{
                printf("\n[OPEN] Masukkan nama file: "); 
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
            openFile(filename);      // Membuka dan memuat isi file ke text_buffer
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
    // 1. pengamanan data jika user sedang mengedit file lain
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
            return; // Batal membuat file baru
        }
        show_message = 0;
    }

    // 2. Alur pembuatan file baru 
   clearBuffer();             // Mengosongkan text_buffer dari sisa teks lama
    initCursor();              // Mengembalikan kursor ke posisi Baris 1, Kolom 1
    strcpy(current_filename, "Untitled"); // Beri nama default dokumen
    is_modified = 0;           // Status baru: belum ada yang perlu disimpan
    handleEditInput("");       // Masuk ke mode editor dengan parameter file kosong
}

// Menangani penyimpanan buffer ke dalam file fisik di disk
void handleSaveAction()
{	// Cek apakah file sudah punya nama (bukan "Untitled")
	if (strlen(current_filename) > 0 && strcmp(current_filename, "Untitled") != 0) 
	{
        saveToFile(current_filename);

        sprintf(bottom_message, "[INFO] Perubahan berhasil disimpan ke file %s", current_filename);
        show_message = 1;
    } 
    // Jika file masih Untitled, lakukan prosedur "Save As"
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
		
		//nama file valid (tidak kosong), disimpan ke disk
        if (strlen(current_filename) > 0) 
		{
            saveToFile(current_filename);

            sprintf(bottom_message, "[INFO] Perubahan berhasil disimpan ke file %s", current_filename);
            show_message = 1;
        }
    }

    // tampilkan pesan + jeda
    strcat(bottom_message, "\nTekan sembarang tombol...");
    renderScreen(text_buffer, total_lines);
    _getch();

    show_message = 0; //Membersihakan baris pesan setelah user menenkan tombol
}

//Menangani penutupan program
void handleExitAction() 
{
	//keluar dari mode edit
    if (is_in_editor == 1) 
	{ 
        if (is_modified == 1) 
		{
            // Peringatan karena ada data yang belum aman
            strcpy(bottom_message, "[WARNING] Perubahan belum disimpan! Tetap keluar? (y/n): ");
        } 
		else 
		{
            // Pesan saat data sudah aman
            strcpy(bottom_message, "[QUIT] Keluar dari Saw<git>? (y/n): ");
        }
        show_message = 1;
        renderScreen(text_buffer, total_lines);
    } 
	else 
	//Jika keluar dari menu utama
	{
		printf("\n[QUIT] Keluar dari Saw<git>? (y/n): ");
    }

    int confirm = _getch();
    if (confirm == 'y' || confirm == 'Y') 
	{
        clearRecovery();
        system("cls");
        exit(0); 
    }
    
    // Reset pesan jika user batal keluar
    show_message = 0;
    strcpy(bottom_message, "");
}

void handleEditInput(char *filename) 
{
	is_in_editor = 1; //Menandai status bahwa user sedang dalam mode edit
	
    if (total_lines == 0) total_lines = 1; //Minimal editor punay 1 baris
    extern char current_filename[256]; //Akses variabel global
    strcpy(current_filename, filename); // set nama file yang sedang aktif dikerjakan

    while (1) 
	{
        renderScreen(text_buffer, total_lines);
        int c = _getch();
        
        if (c == 27) // ESC (Kembali ke menu utma)
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
        else if (c == 224) // Kode awal untuk tombol fungsi (Panah)
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
        else if (c == 19) //Ctrl + S (save/Save As)
		{ 
            handleSaveAction();
        }       
        else if (c == 8)  // Backspace
		{ 
            delete_char();
            markAsModified();
        }
        else if (c == 13) //Enter
		{ 
            insert_newline(); //Tambah baris dari modul buffer
            markAsModified(); 
        } 
        else if (c >= 32 && c <= 126) //Pengetikan karakter standar (huruf, angka, simbol)
		{ 
            insert_char((char)c); //Memasukan karakter ke posisi kursor di text_buffer
            markAsModified(); 
        }
    }
}

// Fungsi Menu ditaruh di bawah
void handleMenuInput() 
{
    int c = _getch(); //Menangkap inputan dari keyboard user

    if (c == '1' || c == 15) //Tekan '1' atau Ctrl + O (Buka File)
	{
		handleOpenAction();
	}
    else if (c == '2' || c == 14) //Tekan '2' atau Ctrl + N (Buat File Baru)
	{
    	handleNewFileAction();
	} 
    else if (c == '3' || c == 9) //Tekan '3' atau Ctrl + I (Informasi Aplikasi)
	{ 
        renderInfoScreen();
        _getch();
    }
    else if (c == '4' || c == 7) //Tekan '4' atau Ctrl + G (Informasi Shortcut)
	{ 
        renderHelpScreen();
        _getch();
    }
    else if (c == '5' || c == 17) //Tekan '5' atau Ctrl + Q (Keluar Program)
	{
		handleExitAction();
	}
}
