#ifndef USEFUL_H
#define USEFUL_H

#include <stdbool.h>
#include <SDL_image.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <SDL_timer.h>
#include <SDL_ttf.h>
#include <string.h>
#include <conio.h>

// Functions
void renderText(SDL_Renderer* renderer, const char* fontFile, const char* text, int x, int y, const char* color, int fontSize);
SDL_Texture* createTexture(SDL_Renderer *renderer, const char *filePath);
bool loadTexture(SDL_Renderer *renderer, const char *imagePath, SDL_Texture **texture);
int readTopScore();
void setTopScore(int newScore);
int initializeSDL();
int createWindowAndRenderer(SDL_Window **window, SDL_Renderer **renderer);

#endif // USEFUL_H