#pragma once
#include"Common.h"

class Button : public Base
{
public:
    Button(SDL_Renderer* renderer, ChooseButton bt);
    Button();
    ~Button();
    ButtonSprite currentSprite;

    void setPosition(const int& x, const int& y);
    void handleEvent(SDL_Event* e);
    void render(SDL_Renderer* renderer);
    void setSize(const int& w, const int& h);
    int getPositionX() { return position.x; };
    int getPositionY() { return position.y; };

    bool inside;
private:
    SDL_Point position;
    SDL_Rect sizer;
    SDL_Rect animation[BUTTON_SPRITE_TOTAL];
};
