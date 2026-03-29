#include <conio.h>
#include "input.h"
#include "buffer.h"
#include "cursor.h"
#include "render.h"
#include "recovery.h"

void handleEditInput()
{
	// Loop utama mode edit: baca tombol dan ubah isi buffer.
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


