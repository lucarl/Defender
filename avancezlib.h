#pragma once

#include "SDL.h"
#include "SDL_ttf.h"

class Sprite {
    SDL_Renderer *renderer;
    SDL_Texture *texture;

public:

    Sprite(SDL_Renderer *renderer, SDL_Texture *texture);

    // Destroys the sprite instance
    void destroy();

    // Draw the sprite at the given position.
    // Valid coordinates are between (0,0) (upper left) and (width-32, height-32) (lower right).
    // (All sprites are 32*32 pixels, clipping is not supported)
    void draw(int x, int y, bool flip);

};

class AvancezLib {
public:
    // Destroys the avancez library instance
    void destroy();

    bool update();

    // Destroys the avancez library instance and exits
    void quit();

    // Creates the main window. Returns true on success.
    bool init(int width, int height);

    // Clears the screen and draws all sprites and texts which have been drawn
    // since the last update call.
    // If update returns false, the application should terminate.
    void processInput();

    void swapBuffers();

    void clearWindow();

    // Create a sprite given a string.
    // All sprites are 32*32 pixels.
    Sprite *createSprite(const char *name);

    // Draws the given text.
    void drawText(int x, int y, const char *msg);

    // Return the total time spent in the game, in seconds.
    float getElapsedTime();

    struct KeyStatus {
        bool fire; // right arrow
        bool up; // w
        bool down; // s
        bool reverse; //space
        bool thrust; //left arrow
        bool bomb; //up arrow
        bool hyperSpace; //down arrow
        bool esc; // escape button
    };

    // Returns the keyboard status. If a flag is set, the corresponding key is being held down.
    void getKeyStatus(KeyStatus &keys);

private:
    SDL_Window *window;
    SDL_Renderer *renderer;

    TTF_Font *font;
    KeyStatus key;
};

