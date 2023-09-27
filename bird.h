#ifndef BIRD_H
#define BIRD_H

#include <stdbool.h>
#include <SDL_image.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <SDL_timer.h>
#include <SDL_ttf.h>
#include <string.h>
#include <conio.h>

// Birds direction - NOT IMPLEMENTED YET
typedef enum {
    TOP_RIGHT,
    TOP_LEFT,
    BOTTOM_RIGHT,
    BOTTOM_LEFT,
    UP,
    DOWN,
    LEFT,
    RIGHT,
    DIRECTION_IDLE,
} BirdDirection;

// Birds states - NOT IMPLEMENTED YET 
typedef enum {
    HITTED,
    FALLING,
    FLYING,
    FLYING_AWAY,
    STATE_IDLE,
} BirdState;

// Bird structure
typedef struct Bird {
    SDL_Texture* texture;     // Texture of the bird
    int x;                    // x-coordinate of the bird
    int y;                    // y-coordinate of the bird
    int destinationX;
    int velocityX;            // Horizontal velocity of the bird
    int velocityY;            // Vertical velocity of the bird
    int frameWidth;           // Width of each animation frame
    int frameHeight;          // Height of each animation frame
    int currentFrame;         // Current frame of animation
    int totalFrames;          // Total number of frames in the spritesheet
    int numColumns;           // Number of columns in the spritesheet
    int numRows;              // Number of rows in the spritesheet
    int frameDelay;           // FrameDelay (to fix the FPS)
    int width, height;
    bool isFlyAway;           // Boolean that indicate if the bird goes away
    bool isHit;               // Boolean that indicate if the bird is hited
    BirdState state;          // Useful to figure out which sprite to render
    BirdDirection direction;  // Figure out the bird's direction
} Bird;

// Variables
extern Uint32 birdHitTime;

// Functions
Bird spawnBird(int spawnX);
void updateBird(Bird *bird);
void renderBird(SDL_Renderer* renderer, Bird *bird);
bool isCursorOnBird(int sightX, int sightY, Bird* bird);
void resetBird(Bird* bird);

// Not in use - Refact
void updateBird_Refact(Bird *bird);

#endif