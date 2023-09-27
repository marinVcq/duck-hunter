#include "game.h"

SDL_Renderer* renderer = NULL;
SDL_Window* window = NULL; 

bool aButtonPressed = false;
bool bButtonPressed = false;
bool startButtonPressed = false;
bool selectButtonPressed = false;
bool joyYButtonPressed = false;
bool joyXButtonPressed = false;

bool menuCursorVisible = true;      // Needed to flick the menu selector
bool popUpVisible = true;      // Needed to flick the menu selector

Bird birds[MAX_BIRDS];              // Define an array of Bird objects
int numBirds = 0;
Uint32 roundStartTime = 0;          // control the pop-pup panel time

// Main function 
int main(int argc, char* argv[]) {

    // Initialization
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
    
    // Clean resource and memory
    cleanupGame();
    return 0;
}

// Function to initialize game variables and SDL
int initializeGame() {

    if (!initializeSDL()) {
        return false;
    }

    if (!createWindowAndRenderer(&window, &renderer)) {
        return false;
    }

    initializeGameVariables();

    // Load game texture and set initial position
    loadTexture(renderer, "sight.png", &sight.sightTexture ); // Replace with your texture loading function
    loadTexture(renderer, "menu.png",&game.menuTexture );
    loadTexture(renderer, "cursor.png",&game.cursorTexture );
    loadTexture(renderer, "background.png",&game.backgroundTexture );
    loadTexture(renderer, "backgroundWithoutSky.png",&game.backgroundWithoutSkyTexture );
    loadTexture(renderer, "sight.png", &sight.sightTexture);
    loadTexture(renderer, "spriteSheet.png", &dog.texture);

    // Intialize Panel
    flyAwayPanel = initializePanel(220,80,PANEL_X,PANEL_Y, "panel.png","FLY AWAY");
    gameOverPanel = initializePanel(250, 80, PANEL_X,PANEL_Y, "panel.png","GAME OVER");
    roundPanel = initializePanel(200,100, PANEL_X,PANEL_Y, "panel.png","ROUND 1");
    perfectPanel = initializePanel(210, 60, PANEL_X,PANEL_Y, "panel.png","PERFECT!");
    tryAgainPanel = initializePanel(600, 100, PANEL_X,PANEL_Y + 150, "flyAway.png","PRESS START TO RESTART");

    // Initialize joystick support
    if (SDL_NumJoysticks() > 0) {
        SDL_JoystickOpen(0); // Open the first joystick
    }

    return 0; // Return 0 on success, -1 on failure
}

void initializeGameVariables(){
    // Initialize game
    game.gameState = MENU;
    game.menuCursorX = 145;
    game.menuCursorY = 414;
    game.gameType = GAME_A;
    game.score = 0;
    game.round = 1;
    game.shot = 10;
    game.roundStart = true;
    game.birdsRemain = 10;
    game.numBirds = 1;
    game.topScore = readTopScore();

    dog.dogState = DOG_IDLE;
    dog.frameDelay = 150;
    dog.dogX = 0 - 180;
    dog.dogY = 430;

    // Initialize sight (Because No NES Zapper :/)
    sight.x = SCREEN_WIDTH / 2 - SIGHT_WIDTH/2;
    sight.y = SCREEN_HEIGHT / 2 - SIGHT_HEIGHT/2;
    sight.width = SIGHT_WIDTH;
    sight.height = SIGHT_HEIGHT;
    sight.speedX = 9;
    sight.speedY = 9;
    sight.isVisible = false;
}
// Function to handle joystick Event
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

            // Handle event in the menu screen
            if (game.gameState == MENU) {

                // Check for "Start" button press (button 8) to start the game
                if (SDL_JoystickGetButton(joystick, START_BUTTON_INDEX)) {
                    game.gameState = PLAYING;
                    roundStartTime = SDL_GetTicks();

                    // depends on the menu Cursor position
                    switch(game.menuCursorY){
                        case 414:
                            game.gameType = GAME_A;
                            break;
                        case 466:
                            game.gameType = GAME_B;
                            break;
                        case 518:
                            game.gameType = GAME_C;
                            break;
                        default:
                            game.gameType = GAME_A;
                    }
                }

                // Check for gameType change
                if (fabs(joyY) > 0.1f) {
                    if (!joyYButtonPressed) {
                        game.menuCursorY += 52;

                        if(game.menuCursorY >= 414 + (3*52)){
                            game.menuCursorY = 414;
                        }
                        joyYButtonPressed = true; // Set the flag to true
                    }
                } else {
                    // Reset the flag when the button is released
                    joyYButtonPressed = false;
                }
            }

            // Handle event during game 
            else if(game.gameState == PLAYING){
                
                // Apply acceleration threshold to joystick input when the game is not started
                if (fabs(joyX) > 0.1f) {
                    sight.x += joyX * sight.speedX;
                }
                if (fabs(joyY) > 0.1f) {
                    sight.y += joyY * sight.speedY;
                }

                // Check for "A" button press (button X, for example) to fire
                bool aButtonPressedNow = SDL_JoystickGetButton(joystick, BUTTON_A) && (game.gameState == PLAYING);
                if (aButtonPressedNow && !aButtonPressed) {

                    bool hitSomething = false;
                    
                    // "A" button was pressed this frame and not pressed in the previous frame
                    for (int i = 0; i < numBirds; i++) {
                        if (isCursorOnBird(sight.x, sight.y, &birds[i]) && !birds[i].isFlyAway) {
                            // Player hit a bird, increase the score depends on the duck type (500 || 1000)
                            game.score += 100;
                            game.hit += 1;
                            birds[i].isHit = true;
                            game.birdsRemain -= 1;
                            birdHitTime = SDL_GetTicks(); 
                            hitSomething = true;
                            break; // Exit the loop after hitting one bird
                        }
                    }

                    if(!hitSomething){
                        game.shot -= 1;
                    }
                }
                
                // Update the flag for next frame
                aButtonPressed = aButtonPressedNow;
            }
            else if(game.gameState == GAME_OVER){
                if (SDL_JoystickGetButton(joystick, START_BUTTON_INDEX)) {
                    game.gameState = PLAYING;
                    roundStartTime = SDL_GetTicks();
                }
            }
        }
    }

    // Clamp the sight position within screen boundaries
    if (sight.x < 0) {
        sight.x = 0;
    } else if (sight.x > SCREEN_WIDTH - SIGHT_WIDTH) {
        sight.x = SCREEN_WIDTH - SIGHT_WIDTH;
    }

    if (sight.y < 0) {
        sight.y = 0;
    } else if (sight.y > SCREEN_HEIGHT - SIGHT_HEIGHT) {
        sight.y = SCREEN_HEIGHT - SIGHT_HEIGHT;
    }

}

// Function to update the game
void updateGame() {

    // 60 FPS
    static Uint32 lastUpdateTime = 0;
    Uint32 currentTime = SDL_GetTicks();

    // Control the frame rate by introducing a time delay
    if (currentTime - lastUpdateTime < 16) {
        // Delay for 16 milliseconds (approximately 60 frames per second)
        SDL_Delay(16 - (currentTime - lastUpdateTime));
    }

    lastUpdateTime = SDL_GetTicks();

    if (game.gameState == MENU) {
        sight.isVisible = false;
        static Uint32 lastCursorToggleTime = 0;
        Uint32 currentTime = SDL_GetTicks();

        // Toggle cursor visibility every 500 milliseconds (0.5 seconds)
        if (currentTime - lastCursorToggleTime >= 500) {
            menuCursorVisible = !menuCursorVisible;
            lastCursorToggleTime = currentTime;
        }
    } 
    
    else if (game.gameState == PLAYING) {

        sight.isVisible = true;
        static bool sniffingStarted = false; // Initialize as false
        Uint32 sniffingStartTime = 0; // Initialize to 0

        if (dogAnimationTerminate == false) {
            dogAnimation(&dog);
        }else{
            if(numBirds < game.numBirds){
                birds[numBirds++] = spawnBird(200); // Set random spawn X
            }

            for(int i=0; i<game.numBirds;i++){
                updateBird(&birds[i]);
            }
        }

        // Handle case NO SHOT REMAIN 
        if(game.shot == 0){
            for(int i=0; i<game.numBirds;i++){
                birds[i].isFlyAway = true;
            }
            // Game Over 
            game.gameState = GAME_OVER;
        }

        // handle round 
        if(game.birdsRemain == 0 && game.shot>0){
            // Go next round 
            game.shot = 3;
            game.hit = 0;
            game.round += 1;
            game.birdsRemain = 10;
            game.roundStart = true;
            roundStartTime = SDL_GetTicks();
        }
    }
    else if (game.gameState == GAME_OVER) {

        static Uint32 lastPopUpToggleTime = 0;
        Uint32 currentTime = SDL_GetTicks();

        // Toggle "try again " popup visibility every second 
        if (currentTime - lastPopUpToggleTime >= 1000) {
            popUpVisible = !popUpVisible;
            lastPopUpToggleTime = currentTime;
        }
        // Check to score
        if (game.score > game.topScore){
            setTopScore(game.score);
        }

    }
}

// Function to render the game
void renderGame() {

    // Clear the screen and render the game
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Render the screen menu
    if (game.gameState == MENU) {

        SDL_Rect menuRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

        SDL_RenderCopy(renderer, game.menuTexture, NULL, &menuRect);

        if (menuCursorVisible) {
            SDL_Rect cursorRect = { game.menuCursorX, game.menuCursorY, CURSOR_WIDTH, CURSOR_HEIGHT };
            SDL_RenderCopy(renderer, game.cursorTexture, NULL, &cursorRect);
        }
        renderTopScore(renderer);
        renderSight(renderer, &sight);

    }
    // Handle Render while playing 
    else if (game.gameState == PLAYING) {

        Uint32 currentTime =  SDL_GetTicks();

        // Render the background
        SDL_Rect backgroundRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderCopy(renderer, game.backgroundTexture, NULL, &backgroundRect);

        // Display round pop-up panel when round begin
        if(game.roundStart){
            char temp[20];
            snprintf(temp, sizeof(temp), "ROUND %d", game.round );
            roundPanel.content = strdup(temp);
            renderPanel(renderer, &roundPanel);
            if((currentTime - roundStartTime) >= 3000){
                game.roundStart = false;
            }
        }

        // Render bird - ONLY ONE AVAILABLE FOR NOW
        for (int i = 0; i < numBirds; i++) {
            if(game.roundStart == false ){
                renderBird(renderer, &birds[i]);
            }
        }

        // Render background layer
        SDL_Rect backgroundNoBgRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderCopy(renderer, game.backgroundWithoutSkyTexture, NULL, &backgroundNoBgRect);

        // Render the dog
        switch(dog.dogState){
            case DOG_WALKING:
                renderDogWalking(renderer, &dog);
                break;
            case DOG_SNIFFING:
                renderDogSniffing(renderer, &dog);
                break;
            case DOG_SMILING:
                renderDogSmiling(renderer, &dog);
                break;
            case DOG_JUMPING:
                renderDogJumping(renderer, &dog);
                break;
            case DOG_LANDING:
                renderDogLanding(renderer, &dog);
                SDL_Rect backgroundRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
                SDL_RenderCopy(renderer, game.backgroundWithoutSkyTexture, NULL, &backgroundRect);
                break;
            default:
                renderDogWalking(renderer, &dog);
        }

        // Render the gun sight
        renderSight(renderer, &sight);

        // Display pop-up panel if bird fly away
        for(int i=0; i<numBirds;i++){
            if(birds[i].isFlyAway){
                renderPanel(renderer, &flyAwayPanel);
            }
        }

        // Render game informations
        renderScore(game.score);
        renderRound(game.round);
        renderShot(game.shot);
        renderHit(game.hit);
    }
    else if (game.gameState == GAME_OVER) {
        Uint32 currentTime =  SDL_GetTicks();

        // Render the background
        SDL_Rect backgroundRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderCopy(renderer, game.backgroundTexture, NULL, &backgroundRect);

        // Render pop-up "GAME OVER"
        renderPanel(renderer, &gameOverPanel);

        // Render pop-up "try again"
        if(popUpVisible){
            renderPanel(renderer, &tryAgainPanel);
        }
    }

    // Update the screen
    SDL_RenderPresent(renderer);
}

// Function to clean
void cleanupGame(){

    SDL_Quit();
}

// Function to render the top score (menu screen)
void renderTopScore(SDL_Renderer *renderer) {
    int topScore = readTopScore();
    char scoreString[50];
    snprintf(scoreString, sizeof(scoreString), "%d", topScore);
    renderText(renderer, "prstart.ttf", scoreString, TOP_SCORE_X, TOP_SCORE_Y, "GREEN", 26);
}

// Function to render the current score
void renderScore(int score){
    char scoreString[50];
    snprintf(scoreString, sizeof(scoreString), "%d", score);
    renderText(renderer, "prstart.ttf", scoreString, SCORE_X, SCORE_Y, "WHITE", 25);
}

// Function to render player's shot remaining
void renderShot(int shot){

    int width =0;
    int height = 0;

    switch(shot){
        case 0:
            width = 70;
            height = 25;
            break;       
        case 1:
            width = 50;
            height = 25;
            break;
        case 2:
            width = 25;
            height = 25;
            break;
        case 3:
            width = 0;
            height = 0;
            break;
    }
    SDL_Color blackColor = {0, 0, 0, 255};
    SDL_Rect rect = {SHOT_X, SHOT_Y, width, height};

    SDL_SetRenderDrawColor(renderer, blackColor.r, blackColor.g, blackColor.b, blackColor.a);
    SDL_RenderFillRect(renderer, &rect);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
}

// Function to render the current round
void renderRound(int round){
    char roundString[50];
    snprintf(roundString, sizeof(roundString), "%d", round);
    renderText(renderer, "prstart.ttf", roundString, ROUND_X, ROUND_Y, "YELLOW", 25);
}

// Function to render hit remaining
void renderHit(int hit){

}

// Function to render the gun sight
void renderSight(SDL_Renderer *renderer, Sight *sight){
    if(sight->isVisible){
        SDL_Rect rect = {sight->x,sight->y,sight->width ,sight->height};
        SDL_RenderCopy(renderer, sight->sightTexture, NULL, &rect);        
    }
}

