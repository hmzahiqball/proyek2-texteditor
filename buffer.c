#include <string.h>
#include "buffer.h"

char text_buffer[MAX_ROW][MAX_COL];
int total_lines = 0;

void initBuffer() {
    int i;
    for (i = 0; i < MAX_ROW; i++) {
        text_buffer[i][0] = '\0';
    }
    total_lines = 0;
}

void appendLine(const char *input) {
    if (total_lines < MAX_ROW) {
        // Menyalin input user ke baris yang tersedia saat ini
        strncpy(text_buffer[total_lines], input, MAX_COL - 1);
        
        // Memastikan string diakhiri dengan null terminator (safe practice)
        text_buffer[total_lines][MAX_COL - 1] = '\0';
        total_lines++;
    }
}

void clearBuffer() {
    initBuffer();
}