#include <stdio.h>
#include "file_io.h"
#include "buffer.h"

void saveToFile(const char *filename) {
    FILE *fp = fopen(filename, "w");

    if (fp == NULL) {
        printf("Gagal menyimpan ke %s.\n", filename);
        return;
    }

    int i;
    for (i = 0; i < total_lines; i++) {
        fprintf(fp, "%s\n", text_buffer[i]);
    }

    fclose(fp);
    printf("[INFO] File berhasil disimpan ke %s\n", filename);
}