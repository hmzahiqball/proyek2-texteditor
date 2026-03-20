#include <stdio.h>
#include "render.h"

void renderScreen(char buffer[10][100], int rows) {
    // Membersihkan layar dan kembali ke posisi atas (0,0)
    printf("\033[H\033[2J"); 

    // Mencetak isi buffer baris demi baris
    int i;
    for ( i = 0; i < rows; i++) {
        printf("%s\n", buffer[i]); 
    }
    
    // Memberi jarak untuk status bar sederhana
    printf("\n--- Editor Saw<git> | Mode: Preview ---\n");
}

