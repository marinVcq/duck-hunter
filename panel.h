#ifndef PANEL_H
#define PANEL_H

#include <stdbool.h>
#include <SDL_image.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <SDL_timer.h>
#include <SDL_ttf.h>
#include <string.h>
#include <conio.h>

// Panel structure to display smthngs 
typedef struct {
    int width, height;
    int posX, posY;
    SDL_Texture *texture;
    char *content;
} Panel;

// Functions
Panel initializePanel(int width, int height, int posX, int posY, char *filePath, const char *content);
void renderPanel(SDL_Renderer *renderer, Panel *panel);
#endif