#include "game.h"
#include "bird.h"

Uint32 birdHitTime = 0;

// Function to initialize bird
Bird spawnBird(int spawnX) {
    // Bird spawning logic (similar to your initialization code)
    Bird newBird;

    loadTexture(renderer, "spriteSheet.png", &newBird.texture); // Load bird spritesheet texture
    newBird.x = spawnX;
    newBird.y = 400;
    newBird.velocityX = 2 + (rand() % 300) / 100.0;
    newBird.velocityY = 1 + (rand() % 400) / 100.0; 
    newBird.frameWidth = 40; // Width of each frame (assuming 3 columns in the spritesheet)
    newBird.frameHeight = 40; // Height of each frame (assuming 3 rows in the spritesheet)
    newBird.currentFrame = 0; // Initialize current frame
    newBird.totalFrames = 3; // Total number of frames in the spritesheet
    newBird.frameDelay = 75;
    newBird.width = 120;
    newBird.height = 120;
    newBird.isFlyAway = false;
    newBird.isHit = false;
    newBird.state = STATE_IDLE;
    newBird.direction = DIRECTION_IDLE;
    return newBird;
}

// Function to update bird REFACT - NOT IMPLEMENTED YET 
void updateBird_Refact(Bird *bird) {

    // Determine the bird's state based on its position
    if (!bird->isFlyAway && !bird->isHit) {
        if (bird->y > SCREEN_HEIGHT - 150) {
            bird->state = FLYING_AWAY;
        } else {
            bird->state = FLYING;
        }
    } else if (bird->isHit) {
        bird->state = HITTED;
    } else {
        // Bird is resetting after fly away
        bird->state = STATE_IDLE;
    }

    if(bird->state == FLYING){
        switch (bird->direction) {
            case TOP_RIGHT:
                bird->x += bird->velocityX;
                bird->y -= bird->velocityY;
                break;
            case TOP_LEFT:
                bird->x -= bird->velocityX;
                bird->y -= bird->velocityY;
                break;
            case BOTTOM_RIGHT:
                bird->x += bird->velocityX;
                bird->y += bird->velocityY;
                break;
            case BOTTOM_LEFT:
                bird->x -= bird->velocityX;
                bird->y += bird->velocityY;
                break;
            case UP:
                bird->x += 0;
                bird->y -= bird->velocityY;
                break;
            case DOWN:
                bird->x += 0;
                bird->y += bird->velocityY;
                break;
            case LEFT:
                bird->x -= bird->velocityX;
                bird->y += 0;
                break;
            case RIGHT:
                bird->x += bird->velocityX;
                bird->y += 0;
                break;
            case DIRECTION_IDLE:
                // No movement in any direction
                break;
        }
    }
    else if(bird->state == HITTED){
        bird->x += 0;
        bird->y += 4;
    }
    else if(bird->state == FLYING_AWAY){
        bird->x += 0;
        bird->y -= 4;
    }

    // Check if the bird hits the screen borders and reverse direction
    if (bird->state == FLYING && (bird->x <= 125 || bird->x + bird->width >= 600 || bird->y <= 0)) {
        switch (bird->direction) {
            case TOP_RIGHT:
                bird->direction = TOP_LEFT;
                break;
            case TOP_LEFT:
                bird->direction = TOP_RIGHT;
                break;
            case BOTTOM_RIGHT:
                bird->direction = BOTTOM_LEFT;
                break;
            case BOTTOM_LEFT:
                bird->direction = BOTTOM_RIGHT;
                break;
            case UP:
                bird->direction = DOWN;
                break;
            case DOWN:
                bird->direction = UP;
                break;
            case LEFT:
                bird->direction = RIGHT;
                break;
            case RIGHT:
                bird->direction = LEFT;
                break;
            case DIRECTION_IDLE:
                // No change in direction
                break;
        }
    }

    // Handle fly away 
    if (bird->y + bird->height >= SCREEN_HEIGHT - 150 && !bird->isHit) {
        bird->state = FLYING_AWAY;
    }

    // Reset bird if fly away or hit
    if ((bird->state == FLYING_AWAY || bird->state == HITTED) && bird->y < 0 - bird->height) {
        resetBird(bird);
    }
}

// Function to update Bird Currently in use
void updateBird(Bird *bird){
    
    // Update the bird's position
    if(!bird->isFlyAway && !bird->isHit){
        bird->x += bird->velocityX;
        bird->y -= bird->velocityY;        
    }else if(bird->isHit){
        bird->x += 0;
        bird->y += 4;
    }else{
        bird->x += 0;
        bird->y -= 4;
    }

    // Check if the bird hits the left or right screen border
    if ((bird->x <= 125 || bird->x + bird->width >= 600) && !bird->isFlyAway) {
        // Reverse the horizontal velocity to change direction
        bird->velocityX = -bird->velocityX;
    }

    // Check if the bird hits the top or bottom screen border
    if (bird->y <= 0 && !bird->isFlyAway) {
        // Reverse the vertical velocity to change direction
        bird->velocityY = -bird->velocityY;
    }

    // Handle fly away 
    if(bird->y + bird->height >= SCREEN_HEIGHT - 150 && !bird->isHit){
        bird->isFlyAway = true;
    }

    // Reset bird if fly away
    if(bird->isFlyAway && bird->y < 0 - bird->height){
        resetBird(bird);
    }

    // Reset bird if hit
    if(bird->isHit && bird->y + bird->height >= SCREEN_HEIGHT - 150){
        resetBird(bird);
    }
}

// Function to render the Bird depends on bird's state
void renderBird(SDL_Renderer* renderer, Bird *bird) {
    int frameWidth = bird->frameWidth;
    int frameHeight = bird->frameHeight;
    int totalFrames = bird->totalFrames;

    static Uint32 lastFrameTime = 0;
    Uint32 currentTime = SDL_GetTicks();
    static int currentFrame = 0;

    // Update the current frame based on the frame delay
    if (currentTime - lastFrameTime >= bird->frameDelay) {
        currentFrame++;
        if (currentFrame >= totalFrames) {
            currentFrame = 0; // Loop back to the first frame
        }
        lastFrameTime = currentTime;
    }

    double pivotAngle = 0.0; // Default angle

    // Determine the rendering flip based on the direction (velocityX)
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    if (bird->velocityX < 0 && bird->velocityY < 0) {
        // If the bird is moving from right to left, flip horizontally
        flip = SDL_FLIP_HORIZONTAL;
        pivotAngle = -45.0;
    }
    if (bird->velocityX < 0 && bird->velocityY > 0) {
        // If the bird is moving from right to left, flip horizontally
        flip = SDL_FLIP_HORIZONTAL;
    }
    if (bird->velocityX < 0 && bird->velocityY < 0) {
        // If the bird is moving from right to left, flip horizontally
        flip = SDL_FLIP_HORIZONTAL;
        pivotAngle = -45.0;
    }
    if (bird->velocityX > 0 && bird->velocityY < 0) {
        // If the bird is moving from right to left, flip horizontally
        pivotAngle = 45.0;
    }
    if(bird->velocityY > 2 || bird->velocityY < -2){
        if (bird->velocityX < 0 && bird->velocityY < 0) {
            // If the bird is moving from right to left, flip horizontally
            flip = SDL_FLIP_HORIZONTAL;
            pivotAngle = -90.0;
        }
        if (bird->velocityX < 0 && bird->velocityY > 0) {
            // If the bird is moving from right to left, flip horizontally
            flip = SDL_FLIP_HORIZONTAL;
        }
        if (bird->velocityX > 0 && bird->velocityY < 0) {
            // If the bird is moving from right to left, flip horizontally
            pivotAngle = 90.0;
        }
    }


    // Calculate the rotation center (pivot point)
    int pivotX = bird->width / 2; // Adjust as needed
    int pivotY = bird->height / 2; // Adjust as needed

    SDL_Point center = { pivotX, pivotY };
    // printf("velocityX : %d\n", bird->velocityX);
    // printf("velocityY : %d\n", bird->velocityY);

    SDL_Rect srcRect;
    
    // Define the source and destination rectangles for rendering
    SDL_Rect destRect = { bird->x, bird->y, bird->width, bird->width};

    if(!bird->isFlyAway && !bird->isHit){
        if (bird->velocityY == 1 || bird->velocityY == -1 || bird->velocityY == 2 || bird->velocityY == -2 ) {
            SDL_Rect srcRect = { currentFrame * frameWidth, 110, frameWidth, frameHeight };
            SDL_RenderCopyEx(renderer, bird->texture, &srcRect, &destRect, pivotAngle, &center, flip);

        }
        if (bird->velocityY == 3 || bird->velocityY == 4 || bird->velocityY == -3 || bird->velocityY == -4) {
            SDL_Rect srcRect = { currentFrame * frameWidth, 150, frameWidth, frameHeight };
            SDL_RenderCopyEx(renderer, bird->texture, &srcRect, &destRect, pivotAngle, &center, flip);

        }        
    }else if(bird->isHit){
        if(currentTime - birdHitTime >= 200){
            SDL_Rect srcRect = { 40, 230, frameWidth, frameHeight };
            SDL_RenderCopyEx(renderer, bird->texture, &srcRect, &destRect, 0, NULL, SDL_FLIP_NONE);  
        }else{
            SDL_Rect srcRect = { 0, 230, frameWidth, frameHeight };
            SDL_RenderCopyEx(renderer, bird->texture, &srcRect, &destRect, 0, NULL, SDL_FLIP_NONE);              
        }

    }
    else{
        SDL_Rect srcRect = { currentFrame * frameWidth, 190, frameWidth, frameHeight };
        SDL_RenderCopyEx(renderer, bird->texture, &srcRect, &destRect, 0, NULL, SDL_FLIP_NONE);      
    }

}

// Function to check if the gun sight is on a bird
bool isCursorOnBird(int sightX, int sightY, Bird* bird) {
    int birdX = bird->x;
    int birdY = bird->y;
    int birdWidth = bird->width;
    int birdHeight = bird->height;

    // Check if the cursor is within the bounding box of the bird
    if (sightX >= birdX && sightX <= birdX + birdWidth &&
        sightY >= birdY && sightY <= birdY + birdHeight) {
        return true;
    }
    return false;
}

// Function to reset bird's params after it's hit or flying away
void resetBird(Bird* bird) {
    bird->x = 200;
    bird->y = 400;
    bird->velocityX = 2 + (rand() % 300) / 100;
    bird->velocityY = 1 + (rand() % 400) / 100; 
    bird->currentFrame = 0; // Initialize current frame
    bird->isFlyAway = false;     
    bird->isHit = false;   
}
