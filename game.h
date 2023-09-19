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
#include<conio.h>

// Joystick index
#define BUTTON_A 1
#define BUTTON_B 0
#define SELECT_BUTTON_INDEX 8
#define START_BUTTON_INDEX 9

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600


#define TIMER_X 670
#define TIMER_Y 570
#define SCORE_X 125
#define SCORE_Y 570
#define WEAPON_X 575
#define WEAPON_Y 10

#define SIGHT_VELOCITY 20
#define NUM_BIRDS 3
#define BIRD_WIDTH 175

typedef struct {
    int x, y; // Position of the player
    int width;
    int height;
    float speedX, speedY;
    SDL_Texture *sightTexture;
} Sight;

typedef struct Bird {
    SDL_Texture* texture;     // Texture of the bird
    int x;                    // x-coordinate of the bird
    int y;                    // y-coordinate of the bird
    int velocityX;            // Horizontal velocity of the bird
    int velocityY;            // Vertical velocity of the bird
    int frameWidth;           // Width of each animation frame
    int frameHeight;          // Height of each animation frame
    int currentFrame;         // Current frame of animation
    int totalFrames;          // Total number of frames in the spritesheet
    int numColumns;           // Number of columns in the spritesheet
    int numRows;              // Number of rows in the spritesheet
    int frameDelay;
    int width, height;
} Bird;



typedef struct {
    SDL_Texture* texture; // Texture for the weapon image
    int magazine;
    int ammo;
} Weapon;

typedef struct {
    int score;
    Weapon *currentWeapon;
} Player;

// Game states
typedef enum {
    START,
    PLAYING,
    GAME_OVER
} GameState;

extern SDL_Renderer* renderer;
extern SDL_Window* window;

// Define game state and player
GameState gameState = START;
Sight sight;
extern Uint32 messageTimer;
extern bool displayStartMessage;
extern int remainingTime; // Initial time in seconds
extern Uint32 startTime;
// Declare function prototypes for your game functions
int initializeGame();
void cleanupGame();
void handleEvents();
void updateGame();
void renderGame();
bool loadTexture(SDL_Renderer *renderer, const char *imagePath, SDL_Texture **texture);
void renderText(SDL_Renderer* renderer, const char* fontFile, const char* text, int x, int y, const char* color, int fontSize);
void renderTimer(SDL_Renderer *renderer,int remainingTime);
void spawnBird();
void renderScore(SDL_Renderer *renderer,int score);
#endif