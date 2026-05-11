#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "buffer.h"
#include "cursor.h"

LineNode *head = NULL;
LineNode *tail = NULL;

int total_lines = 0;

// Ambil node berdasarkan row
LineNode* getLine(int row) {
    if (row < 0 || row >= total_lines)
        return NULL;

    LineNode *current = head;

    int i;
    for (i = 0; i < row; i++) {
        current = current->next;
    }

    return current;
}

// Inisialisasi Buffer
void initBuffer() {
    head = NULL;
    tail = NULL;

    total_lines = 0;

    // Biar editor tetap punya 1 line kosong
    appendLine("");
}

// Hapus Semua Buffer
void clearBuffer() {
    LineNode *current = head;

    while (current != NULL) {

        LineNode *temp = current;

        current = current->next;

        free(temp);
    }

    head = NULL;
    tail = NULL;

    total_lines = 0;

    appendLine("");
}

// Tambah Line Baru
void appendLine(const char *input) {
    LineNode *newNode;

    newNode = (LineNode*) malloc(sizeof(LineNode));

    if (newNode == NULL)
        return;

    strncpy(newNode->line, input, MAX_COL - 1);

    newNode->line[MAX_COL - 1] = '\0';

    newNode->prev = NULL;
    newNode->next = NULL;

    // Kalau list masih kosong
    if (head == NULL) {
        head = newNode;
        tail = newNode;
    }
    else {
        tail->next = newNode;

        newNode->prev = tail;

        tail = newNode;
    }
    total_lines++;
}

// Insert Character
void insert_char(char c) {
    LineNode *current = getLine(cursor_row);

    if (current == NULL)
        return;

    int len = strlen(current->line);

    // Buffer line penuh
    if (len >= MAX_COL - 1)
        return;

    // Geser karakter ke kanan
    memmove(
        &current->line[cursor_col + 1],
        &current->line[cursor_col],
        len - cursor_col + 1
    );

    current->line[cursor_col] = c;
    cursor_col++;

    limitCursorBounds();
    adjust_viewport();
}

// Delete karakter (Backspace)
void delete_char() {
    LineNode *current = getLine(cursor_row);

    if (current == NULL)
        return;

    // Hapus karakter biasa
    if (cursor_col > 0) {
        int len = strlen(current->line);
        memmove(
            &current->line[cursor_col - 1],
            &current->line[cursor_col],
            len - cursor_col + 1
        );
        cursor_col--;
    }

    // Merge line dengan line sebelumnya
    else if (current->prev != NULL) {
        LineNode *prev = current->prev;
        int prev_len = strlen(prev->line);

        // Cek overflow
        if (prev_len + strlen(current->line) < MAX_COL) {

            strcat(prev->line, current->line);
        }

        // Sambungkan node
        prev->next = current->next;

        if (current->next != NULL) {
            current->next->prev = prev;
        }
        else {
            tail = prev;
        }

        free(current);
        total_lines--;
        cursor_row--;
        cursor_col = prev_len;
    }
    limitCursorBounds();
    adjust_viewport();
}

// Menambahkan newline
void insert_newline() {
    LineNode *current = getLine(cursor_row);

    if (current == NULL)
        return;

    LineNode *newNode;

    newNode = (LineNode*) malloc(sizeof(LineNode));

    if (newNode == NULL)
        return;

    // Copy teks setelah cursor
    strcpy(
        newNode->line,
        &current->line[cursor_col]
    );

    // Potong line lama
    current->line[cursor_col] = '\0';

    // Set pointer node baru
    newNode->prev = current;
    newNode->next = current->next;

    // Kalau ada next
    if (current->next != NULL) {
        current->next->prev = newNode;
    }
    else {
        tail = newNode;
    }

    current->next = newNode;
    total_lines++;
    cursor_row++;
    cursor_col = 0;

    limitCursorBounds();
    adjust_viewport();
}

// Mendapatkan panjang line
int getLineLength(int row) {
    LineNode *current = getLine(row);
    if (current == NULL)
        return 0;

    return strlen(current->line);
}

// Mendapatkan karakter pada posisi tertentu
char getCharAt(int row, int col) {
    LineNode *current = getLine(row);

    if (current == NULL)
        return '\0';

    int len = strlen(current->line);

    if (col < 0 || col >= len)
        return '\0';

    return current->line[col];
}