#ifndef INPUT_H
#define INPUT_H

/**
 * handleMenuInput:
 * Menangani input angka 1-5 di layar utama.
 * Fungsi ini dipanggil di dalam loop main.c.
 */
void handleMenuInput();

/**
 * handleEditInput:
 * Menangani input karakter, tombol panah, dan shortcut
 * saat user berada di dalam mode editor (setelah Open/Create).
 */
void handleEditInput(char *filename);

#endif

