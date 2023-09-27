#include "useful.h"
#include "game.h"

// Function to render text
void renderText(SDL_Renderer* renderer, const char* fontFile, const char* text, int x, int y, const char* color, int fontSize) {

    TTF_Font* font = TTF_OpenFont(fontFile, fontSize);
    SDL_Color greenColor = {92, 228,52};
    SDL_Color whiteColor = {255, 255, 255} ;
    SDL_Color yellowColor = {140,215,1,255} ;
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
    else if(strcmp(color, "YELLOW") == 0){
        textSurface = TTF_RenderText_Solid(font, text, yellowColor);

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

// Function to create an SDL_Texture from an image file
SDL_Texture* createTexture(SDL_Renderer *renderer, const char *filePath) {
    SDL_Texture *texture = NULL;
    SDL_Surface *surface = NULL;

    surface = IMG_Load(filePath);
    if (!surface) {
        fprintf(stderr, "Failed to load image: %s\n", SDL_GetError());
        return NULL;
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        fprintf(stderr, "Failed to create texture: %s\n", SDL_GetError());
    }

    SDL_FreeSurface(surface);
    return texture;
}

// Function to load an SDL_Texture from an image file within a texture variable
bool loadTexture(SDL_Renderer *renderer, const char *imagePath, SDL_Texture **texture) {
    SDL_Surface *surface = IMG_Load(imagePath);
    if (!surface) {
        return false;
    }
    *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return (*texture != NULL);
}

// Read Top score from file
int readTopScore() {
    int maxScore = 0;
    FILE* file = fopen("maxScore.txt", "r");
    if (file != NULL) {
        fscanf(file, "%d", &maxScore);
        fclose(file);
    }
    return maxScore;
}

// Function to set a new top score 
void setTopScore(int newScore) {
    FILE *file = fopen("maxScore.txt", "w");
    if (file == NULL) {
        // Handle the error if the file couldn't be opened
        printf("Error opening file for writing.\n");
        return;
    }
    fprintf(file, "%d", newScore);
    fclose(file);
}

// Initialize SDL
int initializeSDL() {

    // Initialize SDL_2 and Joystick support
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

    return true; // Return true on success, false on failure
}

// Function to create the window and the renderer
int createWindowAndRenderer(SDL_Window **window, SDL_Renderer **renderer) {
    *window = SDL_CreateWindow("NES - Duck Hunter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                               SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!(*window)) {
        printf("Failed to create window: %s\n", SDL_GetError());
        return false;
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (!(*renderer)) {
        printf("Failed to create renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(*window);
        return false;
    }

    return true; // Return true on success, false on failure
}