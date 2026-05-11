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

void insertLineAt(int row, const char *text) {
    // Validasi row
    if (row < 0 || row > total_lines)
        return;

    // Buat node baru
    LineNode *newNode;

    newNode = (LineNode*) malloc(sizeof(LineNode));

    if (newNode == NULL)
        return;

    strncpy(newNode->line, text, MAX_COL - 1);
    newNode->line[MAX_COL - 1] = '\0';

    newNode->prev = NULL;
    newNode->next = NULL;

    // List kosong
    if (head == NULL) {
        head = newNode;
        tail = newNode;
    }

    // Insert di awal
    else if (row == 0) {
        newNode->next = head;
        head->prev = newNode;
        head = newNode;
    }

    // Insert di akhir
    else if (row == total_lines) {
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }

    // Insert di tengah
    else {
        LineNode *current = getLine(row);
        LineNode *before = current->prev;

        // Sambung kiri
        before->next = newNode;
        newNode->prev = before;

        // Sambung kanan
        newNode->next = current;
        current->prev = newNode;
    }

    total_lines++;
}

void deleteLineAt(int row) {
    // Validasi
    if (row < 0 || row >= total_lines)
        return;

    LineNode *current = getLine(row);

    if (current == NULL)
        return;

    // Hanya 1 node
    if (head == tail) {
        head = NULL;
        tail = NULL;
    }

    // Delete head
    else if (current == head) {
        head = current->next;
        head->prev = NULL;
    }

    // Delete tail
    else if (current == tail) {
        tail = current->prev;
        tail->next = NULL;
    }

    // Delete tengah
    else {
        current->prev->next = current->next;
        current->next->prev = current->prev;
    }

    // Free memory
    free(current);
    total_lines--;
}

// Insert Character
void insert_char(char c) {
    LineNode *current = getLine(cursor_row);

    if (current == NULL)
        return;

    int len = strlen(current->line);

    if (cursor_col < 0 || cursor_col > len)
        return;

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

    // Backspace biasa
    if (cursor_col > 0) {
        int len = strlen(current->line);
        memmove(
            &current->line[cursor_col - 1],
            &current->line[cursor_col],
            len - cursor_col + 1
        );
        cursor_col--;
    }

    // Merge line
    else if (current->prev != NULL) {
        LineNode *prev = current->prev;
        int prev_len = strlen(prev->line);

        // Cegah overflow
        if (prev_len + strlen(current->line) < MAX_COL) {
            // Gabung isi line
            strcat(prev->line, current->line);
            deleteLineAt(cursor_row);
            cursor_row--;
            cursor_col = prev_len;
        }

        // Delete current line
        deleteLineAt(cursor_row);
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

    char newLineText[MAX_COL];

    // Copy teks setelah cursor
    strcpy(
        newLineText,
        &current->line[cursor_col]
    );

    // Potong line lama
    current->line[cursor_col] = '\0';

    // Insert line baru
    insertLineAt(
        cursor_row + 1,
        newLineText
    );

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