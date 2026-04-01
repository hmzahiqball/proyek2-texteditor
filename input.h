#ifndef INPUT_H
#define INPUT_H

/**
 * handleMenuInput:
 * Menangani input angka 1-5 di layar utama.
 */
void handleMenuInput();

/**
 * handleEditInput:
 * Menangani input saat dalam mode editor.
 * Parameter filename digunakan untuk membedakan Save (Open) atau Save As (Create).
 */
void handleEditInput(char *filename);

#endif
