#ifndef RENDER_H
#define RENDER_H

#include "buffer.h"

void renderMainMenu();

void renderInfoScreen();

void renderHelpScreen();

void renderScreen(char buffer[MAX_ROW][MAX_COL], int rows);

#endif
