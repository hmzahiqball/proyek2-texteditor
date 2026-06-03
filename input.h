#ifndef INPUT_H
#define INPUT_H

extern int is_in_editor;

void clearBottomMessage();

int askConfirmation(const char *warning_text);

void handleOpenAction();

void handleNewFileAction();

void handleSaveAsAction();

void handleSaveAction();

void handleExitAction();

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
