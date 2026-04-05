#ifndef RENDER_H
#define RENDER_H
#define SCREEN_HEIGHT 20
#define SCREEN_WIDTH  80

#include "buffer.h"

extern int view_row_offset;
extern int view_col_offset;

void renderMainMenu();

void renderInfoScreen();

void renderHelpScreen();

void renderScreen(char *buffer[MAX_ROW], int rows);

#endif
