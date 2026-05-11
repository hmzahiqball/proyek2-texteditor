#ifndef BUFFER_H
#define BUFFER_H

#define MAX_COL 100

typedef struct LineNode {
    char line[MAX_COL];

    struct LineNode *prev;
    struct LineNode *next;
} LineNode;

extern LineNode *head;
extern LineNode *tail;

extern int total_lines;

// inisialisasi dan clear buffer
void initBuffer();
void clearBuffer();

// Penambahan dan pengambilan baris
void appendLine(const char *input);
LineNode* getLine(int row);

// Manipulasi karakter
void insert_char(char c);
void delete_char();
void insert_newline();

// Fungsi tambahan
int getLineLength(int row);
char getCharAt(int row, int col);

#endif