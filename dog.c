#include "dog.h"

// Variables
bool dogAnimationTerminate = false;

// Function to annimate dog when game start
void dogAnimation(Dog *dog){
    static bool step1isDone = false;
    static bool step2isDone = false;
    static bool step3isDone = false;
    static bool step4isDone = false;
    static bool step5isDone = false;
    static bool step6isDone = false;
    static bool step7isDone = false;
    static bool step8isDone = false;

    static bool step2Started = false;
    static bool step3Started = false;
    static bool step4Started = false;
    static bool step5Started = false;
    static bool step6Started = false;
    static bool step7Started = false;
    static bool step8Started = false;

    Uint32 startTime = 0;

    if(step1isDone == false){
        dog->dogState = DOG_WALKING;
        dog->dogX += 2;
        if (dog->dogX > 100) {
            step1isDone = true;
            step2Started = true;
        }
    }
    if(step2isDone == false && step2Started == true){
        dog->dogState = DOG_SNIFFING;
        Uint32 currentTime = SDL_GetTicks();
        if((currentTime - startTime) >= 5000){
            step2isDone = true;
            step3Started = true;
        }
    }
    if(step3isDone == false && step3Started == true){
        dog->dogState = DOG_WALKING;
        dog->dogX += 2;
        if (dog->dogX > 200) {
            step3isDone = true;
            step4Started = true;
        }
    }
    if(step4isDone == false && step4Started == true){
        dog->dogState = DOG_SNIFFING;
        Uint32 currentTime = SDL_GetTicks();
        if((currentTime - startTime) >= 7000){
            step4isDone = true;
            step5Started = true;
        }
    }
    if(step5isDone == false && step5Started == true){
        dog->dogState = DOG_SMILING;
        Uint32 currentTime = SDL_GetTicks();
        if((currentTime - startTime) >= 8000){
            step5isDone = true;
            step6Started = true;
        }
    }
    if(step6isDone == false && step6Started == true){
        dog->dogState = DOG_JUMPING;
        dog->dogX += 1;
        dog->dogY -= 7;
        if (dog->dogX > 225 && dog->dogY <225) {
            step6isDone = true;
            step7Started = true;
        }
    }
    if(step7isDone == false && step7Started == true){
        dog->dogState = DOG_LANDING;
        dog->dogX += 4;
        dog->dogY += 7;
        if (dog->dogX > 265 && dog->dogY >= 425) {
            step7isDone = true;
            dogAnimationTerminate = true;
        }
    }
}

// Functions to render the dog depending on dog's state WALKING|SMILING|SNIFFING|JUMPING|LANDING | REFACT NEEDED
void renderDogWalking(SDL_Renderer* renderer, Dog *dog) {

    int x = dog->dogX;
    int y = dog->dogY;

    int frameWidth = 60;    // Width of each frame in pixels
    int frameHeight = 48;   // Height of each frame in pixels
    int totalFrames = 4;    // Total number of frames for walking animation

    static Uint32 lastFrameTime = 0;
    Uint32 currentTime = SDL_GetTicks();
    static int currentFrame = 0;

    // Update the current frame based on the frame delay
    if (currentTime - lastFrameTime >= dog->frameDelay) {
        currentFrame++;
        if (currentFrame >= totalFrames) {
            currentFrame = 0; // Loop back to the first frame
        }
        lastFrameTime = currentTime;
    }

    // Define the source and destination rectangles for rendering
    SDL_Rect srcRect = { currentFrame * frameWidth, 0, frameWidth, frameHeight };
    SDL_Rect destRect = { x, y, frameWidth *3, frameHeight*3 };

    // Render the dog's walking frame
    SDL_RenderCopy(renderer, dog->texture, &srcRect, &destRect);
}

void renderDogSniffing(SDL_Renderer* renderer, Dog *dog) {

    int x = dog->dogX;
    int y = dog->dogY;

    int frameWidth = 60;
    int frameHeight = 48;
    int totalFrames = 2;    // Total number of frames for sniffing animation

    static Uint32 lastFrameTime = 0;
    Uint32 currentTime = SDL_GetTicks();
    static int currentFrame = 0;

    // Update the current frame based on the frame delay
    if (currentTime - lastFrameTime >= dog->frameDelay) {
        currentFrame++;
        if (currentFrame >= totalFrames) {
            currentFrame = 0; // Loop back to the first frame
        }
        lastFrameTime = currentTime;
    }

    // Define the source and destination rectangles for rendering
    SDL_Rect srcRect = { currentFrame * frameWidth + 180, 0, frameWidth, frameHeight };
    SDL_Rect destRect = { x, y, frameWidth *3, frameHeight*3 };

    // Render the dog's sniffing frame
    SDL_RenderCopy(renderer, dog->texture, &srcRect, &destRect);
}
void renderDogSmiling(SDL_Renderer* renderer, Dog *dog){

    int x = dog->dogX;
    int y = dog->dogY;

    int frameWidth = 60;
    int frameHeight = 48;
    // Define the source and destination rectangles for rendering
    SDL_Rect srcRect = { 0,60, frameWidth, frameHeight };
    SDL_Rect destRect = { x, y-6, frameWidth *3, frameHeight*3 };

    // Render the dog's smiling frame
    SDL_RenderCopy(renderer, dog->texture, &srcRect, &destRect);
}
void renderDogJumping(SDL_Renderer* renderer, Dog *dog){

    int x = dog->dogX;
    int y = dog->dogY;

    int frameWidth = 60;
    int frameHeight = 48;
    // Define the source and destination rectangles for rendering
    SDL_Rect srcRect = { 60, 55, frameWidth, frameHeight };
    SDL_Rect destRect = { x, y, frameWidth *3, frameHeight*3 };

    // Render the dog's jumping frame
    SDL_RenderCopy(renderer, dog->texture, &srcRect, &destRect);
}
void renderDogLanding(SDL_Renderer* renderer, Dog *dog){

    int x = dog->dogX;
    int y = dog->dogY;

    int frameWidth = 60;
    int frameHeight = 48;
    // Define the source and destination rectangles for rendering
    SDL_Rect srcRect = { 120, 55, frameWidth, frameHeight };
    SDL_Rect destRect = { x, y, frameWidth *3, frameHeight*3 };

    double angle = 45.0;

    // Render the dog's landing frame
    SDL_RenderCopyEx(renderer, dog->texture, &srcRect, &destRect, angle, NULL, SDL_FLIP_NONE);
}