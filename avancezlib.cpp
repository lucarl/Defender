#include <string>
#include <map>
#include "avancezlib.h"

SDL_Surface *surface, *temp;
SDL_Texture *texture;
SDL_RendererFlip flip = SDL_FLIP_HORIZONTAL;

std::map<int, bool> keyboard;


bool AvancezLib::init(int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return 1;
    }

    TTF_Init();

    window = SDL_CreateWindow
            ("Defender", // window's title
             10, 25, // coordinates on the screen, in pixels, of the window's upper left corner
             width, height, // window's length and height in pixels
             SDL_WINDOW_OPENGL);
    renderer = SDL_CreateRenderer(window, -1, 0);

    font = TTF_OpenFont("space_invaders.ttf", 18);

    return 0;
}

void AvancezLib::destroy() {

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);


    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);

    TTF_Quit();
    SDL_Quit();
}

float AvancezLib::getElapsedTime() {
    static unsigned int currentTime;
    currentTime = SDL_GetTicks();
    return currentTime;
}

void AvancezLib::drawText(int x, int y, const char *msg) {
    SDL_Color black = {0, 0,
                       0};  // this is the color in rgb format, maxing out all would give you the color white, and it will be your text's color

    SDL_Color white = {255, 255, 255};

    SDL_Surface *surf = TTF_RenderText_Solid(font, msg,
                                             white); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first

    SDL_Texture *msg_texture = SDL_CreateTextureFromSurface(renderer, surf); //now you can convert it into a texture

    int w = 0;
    int h = 0;
    SDL_QueryTexture(msg_texture, NULL, NULL, &w, &h);
    SDL_Rect dst_rect = {x, y, w, h};

    SDL_RenderCopy(renderer, msg_texture, NULL, &dst_rect);

    SDL_DestroyTexture(msg_texture);
    SDL_FreeSurface(surf);

}

void Sprite::draw(int x, int y, bool reverse) {
    SDL_Rect rect;

    rect.x = x;
    rect.y = y;

    SDL_QueryTexture(texture, NULL, NULL, &(rect.w), &(rect.h));
    //Render texture to screen
    if (reverse) {
        SDL_RenderCopyEx(renderer, texture, NULL, &rect, 0, 0, flip);
    } else {
        SDL_RenderCopy(renderer, texture, NULL, &rect);
    }
}

Sprite *AvancezLib::createSprite(const char *name) {
    temp = SDL_LoadBMP(name);
    texture = SDL_CreateTextureFromSurface(renderer, temp);
    Sprite *sprite = new Sprite(renderer, texture);

    SDL_FreeSurface(temp);

    return sprite;
}

Sprite::Sprite(SDL_Renderer *renderer, SDL_Texture *texture) {
    Sprite::renderer = renderer;
    Sprite::texture = texture;
}

void Sprite::destroy() {
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
}

void AvancezLib::getKeyStatus(KeyStatus &keys) {
    if (keyboard[SDLK_w]) {
        keys.up = true;
    }
    if (!keyboard[SDLK_w]) {
        keys.up = false;
    }
    if (keyboard[SDLK_s]) {
        keys.down = true;
    }
    if (!keyboard[SDLK_s]) {
        keys.down = false;
    }
    if (keyboard[SDLK_SPACE]) {
        keys.reverse = true;
    }
    if (!keyboard[SDLK_SPACE]) {
        keys.reverse = false;
    }
    if (keyboard[SDLK_RIGHT]) {
        keys.fire = true;
    }
    if (!keyboard[SDLK_RIGHT]) {
        keys.fire = false;
    }
    if (keyboard[SDLK_LEFT]) {
        keys.thrust = true;
    }
    if (!keyboard[SDLK_LEFT]) {
        keys.thrust = false;
    }
    if (keyboard[SDLK_UP]) {
        keys.bomb = true;
    }
    if (!keyboard[SDLK_UP]) {
        keys.bomb = false;
    }
    if (keyboard[SDLK_DOWN]) {
        keys.hyperSpace = true;
    }
    if (!keyboard[SDLK_DOWN]) {
        keys.hyperSpace = false;
    }
    if (keyboard[SDLK_ESCAPE]) {
        keys.esc = true;
    }
    if (!keyboard[SDLK_ESCAPE]) {
        keys.esc = false;
    }
}

void AvancezLib::processInput() {
    static SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_KEYDOWN:
                keyboard[event.key.keysym.sym] = true;
                getKeyStatus(key);
                break;
            case SDL_KEYUP:
                keyboard[event.key.keysym.sym] = false;
                getKeyStatus(key);
                break;
        }
    }
}

void AvancezLib::swapBuffers() {
    SDL_GL_SwapWindow(window);
    SDL_RenderPresent(renderer);
}

void AvancezLib::clearWindow() {
    SDL_RenderClear(renderer);
}

void AvancezLib::quit() {
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);


    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);

    TTF_Quit();
    SDL_Quit();
}

