#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "buffer.h"
#include "cursor.h"

#define INITIAL_CAPACITY 16

LineNode *head = NULL;
LineNode *tail = NULL;

int total_lines = 0;

// Memastikan kapasitas line cukup
void ensureCapacity(LineNode *node, int needed) {
    if (needed >= node->capacity) {

        while (needed >= node->capacity) {
            node->capacity *= 2;
        }

        char *tmp = (char*) realloc(node->line, node->capacity);
        if (tmp == NULL) {
            return;
        }
        node->line = tmp;
    }
}

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

// Hapus Semua Buffer (tanpa menambahkan baris kosong)
void clearBuffer() {
    LineNode *current = head;

    while (current != NULL) {

        LineNode *temp = current;

        current = current->next;

        free(temp->line);
        free(temp);
    }

    head = NULL;
    tail = NULL;

    total_lines = 0;
}

// Tambah Line Baru
void appendLine(const char *input) {
    LineNode *newNode;

    newNode = (LineNode*) malloc(sizeof(LineNode));

    if (newNode == NULL)
        return;

    newNode->length = strlen(input);
    newNode->capacity = newNode->length + INITIAL_CAPACITY;

    newNode->line = (char*) malloc(newNode->capacity);

    strcpy(newNode->line, input);

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

    newNode->length = strlen(text);
    newNode->capacity = newNode->length + INITIAL_CAPACITY;

    newNode->line = (char*) malloc(newNode->capacity);

    strcpy(newNode->line, text);

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

// Helper: Menghapus node dari linked list (HANYA untuk struktur, bukan karakter)
void removeLineNode(int row) {
    if (row < 0 || row >= total_lines)
        return;

    LineNode *current = getLine(row);
    if (current == NULL)
        return;

    // Lepaskan dari linked list
    if (current->prev != NULL)
        current->prev->next = current->next;
    if (current->next != NULL)
        current->next->prev = current->prev;

    // Update head/tail jika perlu
    if (current == head)
        head = current->next;
    if (current == tail)
        tail = current->prev;

    // Bebaskan memory
    free(current->line);
    free(current);
    total_lines--;
}

// Menghapus baris pada posisi tertentu (wrapper untuk backward compatibility)
void deleteLineAt(int row) {
    if (row < 0 || row >= total_lines)
        return;

    LineNode *current = getLine(row);
    if (current == NULL)
        return;

    // Update global current_line jika menunjuk ke node yang dihapus
    if (current_line == current) {
        if (current->next != NULL) {
            current_line = current->next;
        } else if (current->prev != NULL) {
            current_line = current->prev;
        } else {
            current_line = NULL;
        }
    }

    removeLineNode(row);
}

void insert_char(char c) {
    LineNode *current = getLine(cursor_row);

    if (current == NULL)
        return;

    if (cursor_col < 0 || cursor_col > current->length)
        return;

    /* make sure global current_line stays in sync with the row we operate on */
    current_line = current;

    ensureCapacity(current, current->length + 2);

    /* shift characters (including terminating '\0') to the right */
    memmove(
        current->line + cursor_col + 1,
        current->line + cursor_col,
        (size_t)(current->length - cursor_col + 1)
    );

    current->line[cursor_col] = c;
    current->length++;

    cursor_col++;

    limitCursorBounds();
    adjust_viewport();
}

// Delete karakter (Backspace)
void delete_char() {
    LineNode *current = getLine(cursor_row);

    if (current == NULL)
        return;

    // Case 1: Backspace biasa (cursor di tengah/akhir baris, bukan kolom 0)
    if (cursor_col > 0) {
        memmove(
            &current->line[cursor_col - 1],
            &current->line[cursor_col],
            current->length - cursor_col + 1
        );

        current->length--;
        cursor_col--;
    }
    // Case 2: Backspace di kolom 0 - merge dengan baris sebelumnya
    else if (current->prev != NULL) {
        LineNode *prev = current->prev;
        int prev_len = prev->length;
        int current_row = cursor_row;  // Simpan baris saat ini sebelum di-update

        // Pastikan prev punya kapasitas untuk menampung current
        ensureCapacity(
            prev,
            prev->length + current->length + 1
        );

        // Copy isi current ke akhir prev (dengan null terminator)
        memcpy(
            prev->line + prev->length,
            current->line,
            (size_t)current->length + 1
        );

        prev->length += current->length;

        // Update cursor ke akhir baris sebelumnya SEBELUM delete node
        current_line = prev;
        cursor_row--;
        cursor_col = prev_len;

        // Hapus node current dari linked list (gunakan original row)
        removeLineNode(current_row);
    }

    limitCursorBounds();
    adjust_viewport();
}

// Delete karakter (Delete key)
void delete_forward() {
    LineNode *current = getLine(cursor_row);

    if (current == NULL)
        return;

    // Case 1: Delete karakter di posisi cursor
    if (cursor_col < current->length) {

        memmove(
            &current->line[cursor_col],
            &current->line[cursor_col + 1],
            current->length - cursor_col
        );

        current->length--;
    }

    // Case 2: Cursor di akhir line -> merge dengan line berikutnya
    else if (current->next != NULL) {

        LineNode *next = current->next;
        int next_row = cursor_row + 1;

        ensureCapacity(
            current,
            current->length + next->length + 1
        );

        memcpy(
            current->line + current->length,
            next->line,
            (size_t)next->length + 1
        );

        current->length += next->length;

        removeLineNode(next_row);
    }

    limitCursorBounds();
    adjust_viewport();
}

// Menambahkan newline
void insert_newline() {
    LineNode *current = getLine(cursor_row);

    if (current == NULL)
        return;

    char *newLineText;

    newLineText = (char*) malloc(
        current->length - cursor_col + 1
    );

    strcpy(
        newLineText,
        &current->line[cursor_col]
    );

    // Potong line lama
    current->line[cursor_col] = '\0';

    current->length = cursor_col;

    // Insert line baru
    insertLineAt(
        cursor_row + 1,
        newLineText
    );

    current_line = current->next;

    free(newLineText);

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

    return current->length;
}

// Mendapatkan karakter pada posisi tertentu
char getCharAt(int row, int col) {
    LineNode *current = getLine(row);

    if (current == NULL)
        return '\0';

    if (col < 0 || col >= current->length)
        return '\0';

    return current->line[col];
}