#ifndef RENDER_H
#define RENDER_H
#define SCREEN_HEIGHT 20
#define SCREEN_WIDTH  80

#include "buffer.h"

extern int view_row_offset;
extern int view_col_offset;
extern int input_mode;

extern char bottom_message[256];
extern int show_message;

void renderMainMenu();
void renderScreen(char buffer[MAX_ROW][MAX_COL], int rows);
void renderInfoScreen();
void renderHelpScreen();

#endif
