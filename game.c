#include "game.h"

SDL_Renderer* renderer = NULL;
SDL_Texture *backgroundTexture = NULL;
SDL_Window* window = NULL; 

Uint32 messageTimer = 0;
bool displayStartMessage = true;
int remainingTime = 60; // Initial time in seconds
Uint32 startTime = 0;
int minX = 0;
int minY = 0;
int maxX = SCREEN_WIDTH - 100; // Subtract cursor width
int maxY = SCREEN_HEIGHT - 100; // Subtract cursor height
int score = 0;

Bird birds[NUM_BIRDS]; // Define an array of Bird objects
int numBirds = 0;
Uint32 lastBirdSpawnTime = 0; // Initialize the timer variable
const Uint32 birdSpawnDelay = 1000; // Delay in milliseconds between bird spawns
// Initialize the weapon
Weapon shotgun;
Player player;

int main(int argc, char* argv[]) {
    if (initializeGame() != 0) {
        fprintf(stderr, "Game initialization failed\n");
        return 1;
    }

    // Main game loop
    int quit = 0;
    SDL_Event e;
    while (!quit) {


        handleEvents(&quit);
        updateGame();
        renderGame();

    }

    cleanupGame();
    return 0;
}

// Define a function to spawn a bird
void spawnBird() {
    // Bird spawning logic (similar to your initialization code)
    Bird newBird;

    loadTexture(renderer, "bird.png", &newBird.texture); // Load bird spritesheet texture
    newBird.x = 0 - BIRD_WIDTH;
    newBird.y = rand() % (321);
    newBird.velocityX = 1 + (rand() % 150) / 100.0; // Random value between 1 and 2
    newBird.velocityY = 0; // Vertical velocity (adjust as needed)
    newBird.frameWidth = 520 / 3; // Width of each frame (assuming 3 columns in the spritesheet)
    newBird.frameHeight = 480 / 3; // Height of each frame (assuming 3 rows in the spritesheet)
    newBird.currentFrame = 0; // Initialize current frame
    newBird.totalFrames = 9; // Total number of frames in the spritesheet
    newBird.numColumns = 3; // Number of columns in the spritesheet
    newBird.numRows = 3; // Number of rows in the spritesheet
    newBird.frameDelay = 20;
    newBird.width = 50;
    newBird.height = 50;

    if (numBirds < NUM_BIRDS) {
        birds[numBirds++] = newBird;
    }
    printf("number of bird: %d\n", numBirds);
}


int initializeGame() {

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) != 0) {
        printf("Failed to initialize SDL: %s\n", SDL_GetError());
        return false;
    }

    // Initialize SDL_image with PNG support
    if (IMG_Init(IMG_INIT_PNG) == 0) {
        printf("Failed to initialize SDL_image: %s\n", IMG_GetError());
        return false;
    }

    // Initialize SDL_ttf
    if (TTF_Init() < 0) {
        printf("Failed to initialize SDL_ttf: %s\n", TTF_GetError());
        return false;
    }

    window = SDL_CreateWindow("Drone Simulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                               SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Failed to create window: %s\n", SDL_GetError());
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Failed to create renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        return false;
    }


    // Load sight texture and set initial position
    loadTexture(renderer, "sight.png", &sight.sightTexture ); // Replace with your texture loading function
    loadTexture(renderer, "background.png",&backgroundTexture );
    loadTexture(renderer, "shotgun.png", &shotgun.texture);

    player.currentWeapon = &shotgun; // Start with the shotgun

    sight.x = SCREEN_WIDTH / 2;
    sight.y = SCREEN_HEIGHT / 2;
    sight.speedX = 6;
    sight.speedY = 6;


    // Initialize joystick support
    if (SDL_NumJoysticks() > 0) {
        SDL_JoystickOpen(0); // Open the first joystick
    }

    gameState = START;
    messageTimer = SDL_GetTicks(); // Get the current time in milliseconds
    displayStartMessage = true;

    return 0; // Return 0 on success, -1 on failure
}

void handleEvents(int* quit) {
    // Poll joystick events
    SDL_JoystickUpdate();
    int numJoysticks = SDL_NumJoysticks();

    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            *quit = 1;
        }
    }

    if (numJoysticks > 0) {
        SDL_Joystick* joystick = SDL_JoystickOpen(0); // Open the first joystick
        if (joystick) {

            // Handle joystick input
            float joyX = SDL_JoystickGetAxis(joystick, 0) / 32767.0f;
            float joyY = SDL_JoystickGetAxis(joystick, 1) / 32767.0f;

            // Check for "Start" button press (button 8) to start the game
            if (SDL_JoystickGetButton(joystick, START_BUTTON_INDEX) && (gameState != PLAYING)) {
                gameState = PLAYING; // Use a single equal sign for assignment
                startTime = SDL_GetTicks();
            }

            // Apply acceleration threshold to joystick input when the game is not started
            if (fabs(joyX) > 0.1f && (gameState == PLAYING)) {
                sight.x += joyX * sight.speedX;
            }
            if (fabs(joyY) > 0.1f && (gameState == PLAYING)) {
                sight.y += joyY * sight.speedY;
            }
            
        }
    }

    // Clamp the cursor position within screen boundaries
    if (sight.x < minX) {
        sight.x = minX;
    } else if (sight.x > maxX) {
        sight.x = maxX;
    }

    if (sight.y < minY) {
        sight.y = minY;
    } else if (sight.y > maxY) {
        sight.y = maxY;
    }

    // Implement other event handling logic here
}


void updateGame() {
 if (gameState == PLAYING) {
        // Calculate the elapsed time since the game started
        Uint32 currentTime = SDL_GetTicks();
        Uint32 elapsedTime = currentTime - startTime;

        if (currentTime - lastBirdSpawnTime >= birdSpawnDelay) {
            // Spawn a bird
            if (numBirds < NUM_BIRDS){
                spawnBird();
            }
            lastBirdSpawnTime = currentTime; // Update the last spawn time
        }

        for (int i = 0; i < numBirds; i++) {
            birds[i].x += birds[i].velocityX;

            // Check if the bird has moved off the screen
            if (birds[i].x > SCREEN_WIDTH) {
                // Reset the bird's position to the left edge with a random y-coordinate
                birds[i].x = 0 - BIRD_WIDTH;
                birds[i].y = rand() % (321); // Random value between 160 and 480 (inclusive)

            }

            // Update the frame delay
            birds[i].frameDelay--;

            if (birds[i].frameDelay <= 0) {
                // Update the current frame for animation
                birds[i].currentFrame++;
                if (birds[i].currentFrame >= birds[i].totalFrames) {
                    birds[i].currentFrame = 0; // Loop back to the first frame
                }

                // Reset the frame delay
                birds[i].frameDelay = 20; // Adjust this value to control the animation speed
            }
        }

        // Calculate the remaining time
        remainingTime = 60 - (elapsedTime / 1000); // Convert milliseconds to seconds

        // Check if the game timer has run out
        if (remainingTime <= 0) {
            gameState = GAME_OVER;
            remainingTime = 0;
        }

    }

}

void renderGame() {

    // Clear the screen and render background
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    if (gameState == START) {

        // Render "Press Start" message at the center of the screen
        // You can use SDL functions to render text or display an image
        SDL_Rect backgroundRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_Rect sightRect = {SCREEN_WIDTH/2 - 50,SCREEN_HEIGHT/2 - 50,100,100};
        SDL_Rect weaponRect = {WEAPON_X,WEAPON_Y,184,64};


        SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect);
        SDL_RenderCopy(renderer, sight.sightTexture, NULL, &sightRect);
        SDL_RenderCopy(renderer, player.currentWeapon->texture, NULL, &weaponRect);

        renderTimer(renderer,remainingTime);

        // Calculate the time passed since the start message started flickering
        Uint32 currentTime = SDL_GetTicks();
        Uint32 timeElapsed = currentTime - messageTimer;

        // Render the message only if the time elapsed is less than, say, 1000 milliseconds (1 second)
        if (timeElapsed % 2000 < 1000) {
            displayStartMessage = true;
        } else {
            displayStartMessage = false;
        }

        // Render the message if the displayMessage flag is true
        if (displayStartMessage) {
            char *text = "Press start to play!";
            renderText(renderer, "prstart.ttf", text, SCREEN_WIDTH/2, SCREEN_HEIGHT/2 + 150,"WHITE",32); 
        }

        renderScore(renderer,score);


    }
    else if (gameState == PLAYING) {
        // Render the player, enemies, bullets, and other game objects
        // Render the gun sight at the center of the screen
        SDL_Rect backgroundRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect);


        // Render the birds
        for (int i = 0; i < numBirds; i++) {
            int frameX = (birds[i].currentFrame % birds[i].numColumns) * birds[i].frameWidth;
            int frameY = (birds[i].currentFrame / birds[i].numColumns) * birds[i].frameHeight;

            SDL_Rect birdSrcRect = {
                frameX,           // Calculate the source x-coordinate based on the current frame
                frameY,           // Calculate the source y-coordinate based on the current frame
                birds[i].frameWidth,
                birds[i].frameHeight
            };

            SDL_Rect birdDestRect = {birds[i].x, birds[i].y, birds[i].frameWidth, birds[i].frameHeight};

            SDL_RenderCopy(renderer, birds[i].texture, &birdSrcRect, &birdDestRect);
        }

        SDL_Rect sightRect = {sight.x -50 ,sight.y -50,100,100};
        SDL_RenderCopy(renderer, sight.sightTexture, NULL, &sightRect);

        renderTimer(renderer,remainingTime);
        renderScore(renderer,score);

    }
    else if (gameState == GAME_OVER) {
        // Render game over message and final score
    }

    // Update the screen
    SDL_RenderPresent(renderer);
}

void cleanupGame(){
    // Free background texture
    if (backgroundTexture) {
        SDL_DestroyTexture(backgroundTexture);
    }
    // Free bird textures
    for (int i = 0; i < NUM_BIRDS; i++) {
        if (birds[i].texture) {
            SDL_DestroyTexture(birds[i].texture);
        }
    }

    // Other cleanup logic

    // Quit SDL
    SDL_Quit();
}

bool loadTexture(SDL_Renderer *renderer, const char *imagePath, SDL_Texture **texture) {
    SDL_Surface *surface = IMG_Load(imagePath);
    if (!surface) {
        return false;
    }
    *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return (*texture != NULL);
}

// Function to display text at screen
void renderText(SDL_Renderer* renderer, const char* fontFile, const char* text, int x, int y, const char* color, int fontSize) {
    
    TTF_Font* font = TTF_OpenFont(fontFile, fontSize);
    SDL_Color greenColor = {0, 255, 0} ;
    SDL_Color whiteColor = {255, 255, 255} ;
    SDL_Color blackColor = {0,0,0} ;

    SDL_Surface* textSurface ;


    if(strcmp(color, "GREEN") == 0){
        textSurface = TTF_RenderText_Solid(font, text, greenColor);


    }else if(strcmp(color, "WHITE") == 0){
        textSurface = TTF_RenderText_Solid(font, text, whiteColor);

    }
    else if(strcmp(color, "BLACK") == 0){
        textSurface = TTF_RenderText_Solid(font, text, blackColor);

    }


    if (!font) {
        // Handle font loading error
        fprintf(stderr, "Failed to load font: %s\n", TTF_GetError());
        return;
    }

    if (textSurface == NULL) {
        // Handle error
        fprintf(stderr, "Failed to render text: %s\n", TTF_GetError());
        TTF_CloseFont(font);
        return;
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    if (textTexture == NULL) {
        // Handle error
        fprintf(stderr, "Failed to create texture from surface: %s\n", SDL_GetError());
        SDL_FreeSurface(textSurface);
        TTF_CloseFont(font);
        return;
    }

    int textWidth, textHeight;
    SDL_QueryTexture(textTexture, NULL, NULL, &textWidth, &textHeight);

    // Adjust the position to center the text
    int centerX = x - textWidth / 2;
    int centerY = y - textHeight / 2;

    SDL_Rect renderRect = {centerX, centerY, textWidth, textHeight};
    SDL_RenderCopy(renderer, textTexture, NULL, &renderRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);
}

void renderTimer(SDL_Renderer *renderer,int remainingTime){
    char remainingTimeString[50];
    snprintf(remainingTimeString, sizeof(remainingTimeString), "Timer: %d", remainingTime);
    renderText(renderer, "prstart.ttf", remainingTimeString, TIMER_X, TIMER_Y,"WHITE",26);
}
void renderScore(SDL_Renderer *renderer,int score){
    char scoreString[50];
    snprintf(scoreString, sizeof(scoreString), "Score: %d", score);
    renderText(renderer, "prstart.ttf", scoreString, SCORE_X, SCORE_Y,"WHITE",26);
}

