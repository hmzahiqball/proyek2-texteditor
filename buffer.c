#include <string.h>
#include <stdlib.h>
#include "buffer.h"
#include "cursor.h"

char *text_buffer[MAX_ROW];
int total_lines = 0;
int line_length[MAX_ROW];

void initBuffer() {
    int i;
    for (i = 0; i < MAX_ROW; i++) {
        text_buffer[i] = NULL;
        line_length[i] = 0;
    }
    total_lines = 0;
}

void clearBuffer() {
    int i;
    for (i = 0; i < MAX_ROW; i++) {
        if (text_buffer[i] != NULL) {
            free(text_buffer[i]);
            text_buffer[i] = NULL;
        }
        line_length[i] = 0;
    }
    total_lines = 0;
}

void appendLine(const char *input) {
    if (total_lines >= MAX_ROW) return;

    int len = strlen(input);

    text_buffer[total_lines] = (char*) malloc(len + 1);
    strcpy(text_buffer[total_lines], input);

    line_length[total_lines] = len;
    total_lines++;
}

void insert_char(char c) {
    int len = line_length[cursor_row];
    char *line = text_buffer[cursor_row];

    if (line == NULL) {
        line = (char*) malloc(1);
        line[0] = '\0';
        len = 0;
    }

    // realloc tambah 1 char
    line = realloc(line, len + 2);

    memmove(
        &line[cursor_col + 1],
        &line[cursor_col],
        len - cursor_col + 1
    );

    line[cursor_col] = c;

    text_buffer[cursor_row] = line;
    line_length[cursor_row]++;

    cursor_col++;

    if (cursor_row >= total_lines) {
        total_lines = cursor_row + 1;
    }

    limitCursorBounds();
    adjust_viewport();
}

void delete_char() {
    if (cursor_col > 0) {
        int len = line_length[cursor_row];
        char *line = text_buffer[cursor_row];

        memmove(
            &line[cursor_col - 1],
            &line[cursor_col],
            len - cursor_col + 1
        );

        line = realloc(line, len); // shrink

        text_buffer[cursor_row] = line;
        line_length[cursor_row]--;

        cursor_col--;
    }

    limitCursorBounds();
    adjust_viewport();
}

void insert_newline() {
    if (total_lines >= MAX_ROW) return;

    char *line = text_buffer[cursor_row];
    int len = line_length[cursor_row];

    // split line
    char *new_line = strdup(&line[cursor_col]);

    line[cursor_col] = '\0';
    line = realloc(line, cursor_col + 1);

    // shift pointer 
    int i;
    for (i = total_lines; i > cursor_row + 1; i--) {
        text_buffer[i] = text_buffer[i - 1];
        line_length[i] = line_length[i - 1];
    }

    text_buffer[cursor_row] = line;
    line_length[cursor_row] = cursor_col;

    text_buffer[cursor_row + 1] = new_line;
    line_length[cursor_row + 1] = strlen(new_line);

    total_lines++;

    cursor_row++;
    cursor_col = 0;

    limitCursorBounds();
    adjust_viewport();
}

int getLineLength(int row) {
    if (row < 0 || row >= total_lines) return 0;
    return line_length[row];
}

char getCharAt(int row, int col) {
    if (row < 0 || row >= total_lines) return '\0';
    if (col < 0 || col >= line_length[row]) return '\0';
    return text_buffer[row][col];
}