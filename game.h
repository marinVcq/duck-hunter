#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <SDL_image.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <SDL_timer.h>
#include <SDL_ttf.h>
#include <string.h>
#include <conio.h>

#include "dog.h"
#include "bird.h"
#include "panel.h"
#include "useful.h"

// Joystick index
#define BUTTON_A 1
#define BUTTON_B 0
#define SELECT_BUTTON_INDEX 8
#define START_BUTTON_INDEX 9

// Screen dimensions
#define SCREEN_WIDTH 768
#define SCREEN_HEIGHT 720

// Position of the top score
#define TOP_SCORE_X 525
#define TOP_SCORE_Y 604

// Position of the current score 
#define SCORE_X 600
#define SCORE_Y 638

// Position of current round
#define ROUND_X 134
#define ROUND_Y 588

// Position of the pop-up panel
#define PANEL_X SCREEN_WIDTH/2
#define PANEL_Y SCREEN_HEIGHT/2 - 150

// Position of remaining shot
#define SHOT_X 75
#define SHOT_Y 620

// Sight specs
#define SIGHT_VELOCITY 20
#define SIGHT_WIDTH 80
#define SIGHT_HEIGHT 80

// Bird constant variables
#define BIRD_WIDTH 175
#define BIRD_HEIGHT 100
#define MAX_BIRDS 2

// Cursor menu constant variables
#define CURSOR_WIDTH 8 * 4
#define CURSOR_HEIGHT 6 * 4

typedef struct {
    int x, y; // Position of the player
    int width;
    int height;
    float speedX, speedY;
    SDL_Texture *sightTexture;
    bool isVisible;
} Sight;

// Game states
typedef enum {
    MENU,
    PLAYING,
    GAME_OVER
} GameState;

// Game type | Not implemented yet
typedef enum {
    GAME_A,
    GAME_B,
    GAME_C
} GameType;

// Game structure
typedef struct {
    SDL_Texture *menuTexture;
    SDL_Texture *cursorTexture;
    SDL_Texture *backgroundTexture;
    SDL_Texture *backgroundWithoutSkyTexture;
    bool roundStart;
    int numBirds;
    int topScore;
    int score;
    int round;
    int shot;
    int hit;
    int birdsRemain;
    int menuCursorX;
    int menuCursorY;
    GameState gameState;
    GameType gameType;
} Game;

// NOT IMPLEMENTED YET
typedef struct {
    int score;
} Player;

extern SDL_Renderer* renderer;
extern SDL_Window* window;
extern Uint32 roundStartTime;
extern bool aButtonPressed;
extern bool bButtonPressed;
extern bool startButtonPressed;
extern bool selectButtonPressed;
extern bool joyYButtonPressed;
extern bool joyXButtonPressed;

// Declare needed stuff
Game game;
Sight sight;
Dog dog;

// Declare panel
Panel gameOverPanel;
Panel flyAwayPanel;
Panel roundPanel;
Panel perfectPanel;
Panel tryAgainPanel;

// Declare function prototypes for your game functions
int initializeGame();
void cleanupGame();
void handleEvents();
void updateGame();
void renderGame();
void renderTopScore(SDL_Renderer *renderer);
void renderScore(int score);
void renderRound(int round);
void renderShot(int shot);
void renderHit(int hit);
void displayRound(int round);
void renderFlyAway();
void renderSight(SDL_Renderer *renderer, Sight *sight);
void initializeGameVariables();

#endif