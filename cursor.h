#ifndef CURSOR_H
#define CURSOR_H

extern int cursor_row;
extern int cursor_col;

void initCursor();
void limitCursorBounds();
void set_cursor_to_end();

void move_left();
void move_right();
void move_up();
void move_down();

#endif