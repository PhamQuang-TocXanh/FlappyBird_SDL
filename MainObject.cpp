#include"MainObject.h"

Bird::Bird()
{
    pos_x = 10;
    pos_y = SCREEN_HEIGHT / 2;

    collider.x = pos_x;
    collider.y = pos_y;
    collider.w = BIRD_WIDTH;
    collider.h = BIRD_HEIGHT;

    vel_x = 0;
    vel_y = 0;
}

Bird::Bird(SDL_Renderer* renderer)
{
	pos_x = 10;
	pos_y = SCREEN_HEIGHT / 2;

    collider.x = pos_x;
    collider.y = pos_y;
	collider.w = BIRD_WIDTH;
	collider.h = BIRD_HEIGHT;

	vel_x = 0;
	vel_y = 0;
    loadImgFromFile("image/GreenBird.png", renderer);
    SDL_Rect sizer = getIMG();
    for (int i = 0; i < 4; i++) {
        animation[i].x = i * sizer.w / 4;
        animation[i].y = 0;
        animation[i].w = sizer.w / 4;
        animation[i].h = sizer.h;
    }
}

Bird::~Bird()
{
    free();
}

void Bird::handleEvent(SDL_Event& e, Mix_Chunk* jump_sound)
{
    if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
    {
        switch (e.key.keysym.sym)
        {
        case SDLK_UP: case SDLK_w: vel_y -= velocity; break;
        case SDLK_DOWN: case SDLK_s: vel_y += velocity; break;
        case SDLK_LEFT: case SDLK_a: vel_x -= velocity; break;
        case SDLK_RIGHT: case SDLK_d: vel_x += velocity; break;
        case SDLK_SPACE: vel_y -= 15; Mix_PlayChannel(-1, jump_sound, 0); gravity = 0; angle = -40; jump = true; break;
        }
    }
    else if (e.type == SDL_KEYUP && e.key.repeat == 0)
    {
        switch (e.key.keysym.sym)
        {
        case SDLK_UP: case SDLK_w: vel_y += velocity; break;
        case SDLK_DOWN: case SDLK_s: vel_y -= velocity; break;
        case SDLK_LEFT: case SDLK_a: vel_x += velocity; break;
        case SDLK_RIGHT: case SDLK_d: vel_x -= velocity; break;
        case SDLK_SPACE: vel_y += 15; gravity = 0; angle = 1; jump = false; break;
        }
    }
}

void Bird::move(Obstacles& obstacle)
{
    pos_x += vel_x;
    collider.x = pos_x;
    if (pos_x<0 || pos_x + BIRD_WIDTH>SCREEN_WIDTH || checkCollision(collider,obstacle.getTopBlock(),obstacle.getBottomBlock())) {
        pos_x -= vel_x;
        collider.x = pos_x;
        dead = true;
    }

    pos_y += vel_y;
    collider.y = pos_y;
    if (pos_y<0 || pos_y + BIRD_HEIGHT>SCREEN_HEIGHT || checkCollision(collider, obstacle.getTopBlock(), obstacle.getBottomBlock())) {
        pos_y -= vel_y; gravity = 0;
        collider.y = pos_y;
        dead = true;
    }
}

void Bird::move(std::vector<Obstacles> ob)
{
    pos_x += vel_x;
    collider.x = pos_x;
    if (pos_x<0 || pos_x + BIRD_WIDTH>SCREEN_WIDTH) {
        pos_x -= vel_x;
        collider.x = pos_x;
        dead = true;
    }

    pos_y += vel_y;
    collider.y = pos_y;
    if (pos_y<0 || pos_y + BIRD_HEIGHT>SCREEN_HEIGHT ) {
        pos_y -= vel_y; gravity = 0;
        collider.y = pos_y;
        dead = true;
    }

    for (std::vector<Obstacles>::iterator i = ob.begin(); i != ob.end(); i++) {
        if (checkCollision(collider, (*i).getTopBlock(), (*i).getBottomBlock()) == true) {
            pos_x -= vel_x; pos_y -= vel_y;
            collider.x = pos_x; collider.y = pos_y;
            dead = true;
        }
    }
}

void Bird::render(SDL_Renderer* renderer)
{
    frame++; if (frame / 8 >= 4) frame = 0;
    Render(pos_x, pos_y, renderer, &animation[frame / 8], &collider, angle);
}

void Bird::Gravity()
{
    gravity += 0.4;
    pos_y += gravity;
}

bool Bird::Pass_Obstacle(Obstacles& obstacle, Mix_Chunk* pass_sound)
{
    SDL_Rect top = obstacle.getTopBlock(), bottom = obstacle.getBottomBlock();
    if (dead == false && collider.x + collider.w == top.x + BLOCK_WIDTH / 2 && collider.x + collider.w == bottom.x + BLOCK_WIDTH / 2) {
        Mix_PlayChannel(-1, pass_sound, 0);
        return true;
    }
    return false;
}
