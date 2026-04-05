#include <string.h>
#include "buffer.h"
#include "cursor.h"

char text_buffer[MAX_ROW][MAX_COL];
int total_lines = 0;
int line_length[MAX_ROW];

void initBuffer() {
    int i;
    for (i = 0; i < MAX_ROW; i++) {
        text_buffer[i][0] = '\0';
        line_length[i] = 0;
    }
    total_lines = 0;
}

void clearBuffer() {
    int i;
    for (i = 0; i < MAX_ROW; i++) {
        text_buffer[i][0] = '\0';
        line_length[i] = 0;
    }
    total_lines = 0;
}

void appendLine(const char *input) {
    if (total_lines >= MAX_ROW) return;

    strncpy(text_buffer[total_lines], input, MAX_COL - 1);
    text_buffer[total_lines][MAX_COL - 1] = '\0';

    line_length[total_lines] = strlen(text_buffer[total_lines]);

    total_lines++;
}

void insert_char(char c) {
    int len = line_length[cursor_row];

    if (len < MAX_COL - 1) {
        memmove(
            &text_buffer[cursor_row][cursor_col + 1],
            &text_buffer[cursor_row][cursor_col],
            len - cursor_col + 1
        );

        text_buffer[cursor_row][cursor_col] = c;

        line_length[cursor_row]++;
        cursor_col++;

        if (cursor_row >= total_lines) {
            total_lines = cursor_row + 1;
        }

        limitCursorBounds();
    }
    adjust_viewport();
}

void delete_char() {
    if (cursor_col > 0) {
        int len = line_length[cursor_row];
        int i;

        memmove(
            &text_buffer[cursor_row][cursor_col - 1],
            &text_buffer[cursor_row][cursor_col],
            len - cursor_col + 1
        );

        line_length[cursor_row]--;
        cursor_col--;
    } 
    else if (cursor_row > 0) {
        int prev_len = line_length[cursor_row - 1];
        int curr_len = line_length[cursor_row];

        if (prev_len + curr_len < MAX_COL - 1) {
            strcat(text_buffer[cursor_row - 1], text_buffer[cursor_row]);

            line_length[cursor_row - 1] += curr_len;

            int i;
            for (i = cursor_row; i < total_lines - 1; i++) {
                strcpy(text_buffer[i], text_buffer[i + 1]);
                line_length[i] = line_length[i + 1];
            }

            text_buffer[total_lines - 1][0] = '\0';
            line_length[total_lines - 1] = 0;

            total_lines--;
            cursor_row--;
            cursor_col = prev_len;
        }
    }

    limitCursorBounds();
    adjust_viewport();
}

void insert_newline() {
    if (total_lines >= MAX_ROW) return;

    if (total_lines == 0) total_lines = 1;

    int i;
    for (i = total_lines; i > cursor_row; i--) {
        strcpy(text_buffer[i], text_buffer[i - 1]);
        line_length[i] = line_length[i - 1];
    }

    strcpy(text_buffer[cursor_row + 1], &text_buffer[cursor_row][cursor_col]);

    line_length[cursor_row + 1] = strlen(text_buffer[cursor_row + 1]);

    text_buffer[cursor_row][cursor_col] = '\0';
    line_length[cursor_row] = cursor_col;

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