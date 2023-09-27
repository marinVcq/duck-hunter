#ifndef DOG_H
#define DOG_H

// Include needed
#include <stdbool.h>
#include <SDL_image.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <SDL_timer.h>
#include <SDL_ttf.h>
#include <string.h>
#include <conio.h>

// Dog state for animation
typedef enum {
    DOG_WALKING,
    DOG_STOPPED,
    DOG_JUMPING,
    DOG_LANDING,
    DOG_SNIFFING,
    DOG_SMILING,
    DOG_IDLE // Add an idle state when the animation is not active
} DogState;

// Dog structure
typedef struct {
    DogState dogState;
    SDL_Texture* texture;
    int dogFrameWidth;
    int dogFrameHeight;
    int dogCurrentFrame;
    int dogTotalFrames;
    int frameDelay;
    int dogX;
    int dogY; // Initial Y position of the dog    
}Dog;

// Useful Variables
extern bool dogAnimationTerminate;

// Functions 
void dogAnimation(Dog *dog);
void renderDogWalking(SDL_Renderer* renderer, Dog *dog);
void renderDogSniffing(SDL_Renderer* renderer, Dog *dog);
void renderDogJumping(SDL_Renderer* renderer, Dog *dog);
void renderDogSmiling(SDL_Renderer* renderer, Dog *dog);
void renderDogLanding(SDL_Renderer* renderer, Dog *dog);

#endif // DOG_H