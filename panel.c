#include "panel.h"
#include "game.h"

// Function to initialize a panel
Panel initializePanel(int width, int height, int posX, int posY, char *filePath, const char *content) {
    Panel panel;
    panel.width = width;
    panel.height = height;
    panel.posX = posX;
    panel.posY = posY;
    loadTexture(renderer,filePath, &panel.texture);
    panel.content = strdup(content);
    return panel;
}

// Function to render the panel on screen
void renderPanel(SDL_Renderer *renderer, Panel *panel){
    char panelString[50];
    snprintf(panelString, sizeof(panelString), panel->content);

    int panelPosX = panel->posX - (panel->width /2);
    int panelPosY = panel->posY - (panel->height /2);

    SDL_Rect rect = {panelPosX,panelPosY, panel->width, panel->height};
    SDL_RenderCopy(renderer,panel->texture, NULL, &rect );
    int textPosX = panelPosX + (panel->width/2);
    int textPosY = panelPosY + (panel->height/2);
    renderText(renderer, "prstart.ttf", panelString, textPosX, textPosY, "WHITE", 22 );
}
