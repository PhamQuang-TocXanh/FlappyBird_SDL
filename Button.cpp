#include"Button.h"


void Button::handleEvent(SDL_Event* e) {
    if (e->type == SDL_MOUSEMOTION) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        inside = true;
        if (x < position.x) inside = false;
        if (y < position.y) inside = false;
        if (x > position.x + sizer.w) inside = false;
        if (y > position.y + sizer.h) inside = false;
        if (!inside) {
            currentSprite = BUTTON_SPRITE_MOUSE_OUT;
        }
        else
        {
            currentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
        }
    }
}

Button::Button(SDL_Renderer* renderer, ChooseButton bt)
{
    position.x = 0; position.y = 0;
    sizer = { 0,0,BUTTON_WIDTH,BUTTON_HEIGHT };
    currentSprite = BUTTON_SPRITE_MOUSE_OUT;
    inside = false;
    if (bt == BUTTON_PLAY) {
        loadImgFromFile("image/PlayIMG.png", renderer);
    }
    else if (bt == BUTTON_EXIT) {
        loadImgFromFile("image/ExitIMG.png", renderer);
    }
    else if (bt == BUTTON_HELP) {
        loadImgFromFile("image/HelpIMG.png", renderer);
    }
    else if (bt == BUTTON_BACK) {
        loadImgFromFile("image/back.png", renderer);
    }else if (bt == BUTTON_PLAY_AGAIN) {
        loadImgFromFile("image/PlayAgainIMG.png", renderer);
    }
    else if (bt == BUTTON_MAP_1) {
        loadImgFromFile("image/map11.png", renderer);
    }
    else if (bt == BUTTON_MAP_2) {
        loadImgFromFile("image/map21.png", renderer);
    }

    SDL_Rect img_size = getIMG();
    for (int i = 0; i < BUTTON_SPRITE_TOTAL; i++) {
        animation[i].x = i * img_size.w / BUTTON_SPRITE_TOTAL;
        animation[i].y = 0;
        animation[i].w = img_size.w / BUTTON_SPRITE_TOTAL;
        animation[i].h = img_size.h;
    }
}

Button::Button()
{
    position.x = 0; position.y = 0;
    sizer = { 0,0,BUTTON_WIDTH,BUTTON_HEIGHT };
    currentSprite = BUTTON_SPRITE_MOUSE_OUT;
    inside = false;
}

Button::~Button()
{
    free();
}

void Button::setPosition(const int& x, const int& y)
{
    position.x = x; position.y = y;
}

void Button::render(SDL_Renderer* renderer)
{
    Render(position.x, position.y, renderer, &animation[currentSprite], &sizer);
}

void Button::setSize(const int& w, const int& h)
{
    sizer.w = w; sizer.h = h;
}
