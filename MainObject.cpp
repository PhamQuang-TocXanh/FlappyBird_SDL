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

Bird::Bird(SDL_Renderer* renderer, int num)
{
	pos_x = 10;
	pos_y = SCREEN_HEIGHT / 2;

    collider.x = pos_x;
    collider.y = pos_y;
	collider.w = BIRD_WIDTH;
	collider.h = BIRD_HEIGHT;

	vel_x = 0;
	vel_y = 0;
    if (num == 1) loadImgFromFile("image/white_bird.png", renderer);
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
    if (dead == false) {
        if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
        {
            switch (e.key.keysym.sym)
            {
            case SDLK_DOWN: case SDLK_s: vel_y += velocity; break;
            case SDLK_LEFT: case SDLK_a: vel_x -= velocity; break;
            case SDLK_RIGHT: case SDLK_d: vel_x += velocity; break;
            case SDLK_SPACE: case SDLK_UP: vel_y -= 15; Mix_PlayChannel(-1, jump_sound, 0); gravity = 0; angle = -30; jump = true; break;
            }
        }
        else if (e.type == SDL_KEYUP && e.key.repeat == 0)
        {
            switch (e.key.keysym.sym)
            {
            case SDLK_DOWN: case SDLK_s: vel_y -= velocity; break;
            case SDLK_LEFT: case SDLK_a: vel_x += velocity; break;
            case SDLK_RIGHT: case SDLK_d: vel_x -= velocity; break;
            case SDLK_SPACE: case SDLK_UP: vel_y += 15; gravity = 0; jump = false; break;
            }
        }
    }
}

void Bird::move(std::vector<Obstacles*> ob)
{
    Gravity();
    pos_x += vel_x;
    collider.x = pos_x;
    if (pos_x + BIRD_WIDTH>SCREEN_WIDTH) {
        pos_x -= vel_x;
        collider.x = pos_x;
    }

    pos_y += vel_y;
    collider.y = pos_y;
    if (pos_y + BIRD_HEIGHT>SCREEN_HEIGHT ) {
        pos_y -= vel_y; gravity = 0;
        collider.y = pos_y;
        dead = true;
    }
    
    for (std::vector<Obstacles*>::iterator i = ob.begin(); i != ob.end(); i++) {
        if (checkCollision(collider, (*i)->getTopBlock(), (*i)->getBottomBlock()) == true) {
            /*std::cout << "x " << (*i)->getTopBlock().x << " y " << (*i)->getTopBlock().y << " w " << (*i)->getTopBlock().w << " h " << (*i)->getTopBlock().h << std::endl;
            std::cout << "co " << collider.x << " " << collider.y << " " << collider.w << " " << collider.h << std::endl;*/
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
    if (angle < 70 && jump == false) angle += 2;
}

bool Bird::Pass_Obstacle(Obstacles obstacle, Mix_Chunk* pass_sound)
{
    SDL_Rect top = obstacle.getTopBlock(), bottom = obstacle.getBottomBlock();
    if (dead == false && collider.x + collider.w == top.x + BLOCK_WIDTH / 2 && collider.x + collider.w == bottom.x + BLOCK_WIDTH / 2) {
        Mix_PlayChannel(-1, pass_sound, 0);
        return true;
    }
    return false;
}

bool Bird::Pass_Obstacle(Obstacles* obstacle, Mix_Chunk* pass_sound)
{
    SDL_Rect top = obstacle->getTopBlock(), bottom = obstacle->getBottomBlock();
    if (collider.x + collider.w >= top.x + BLOCK_WIDTH / 2 && collider.x + collider.w < top.x + BLOCK_WIDTH) inScoreRange = true;
    else if (collider.x + collider.w >= top.x + BLOCK_WIDTH) inScoreRange = false;
    if (dead == false && inScoreRange == true) {//(collider.x + collider.w == top.x + BLOCK_WIDTH / 2 || collider.x + collider.w == 1 + top.x + BLOCK_WIDTH / 2 || collider.x + collider.w == top.x + BLOCK_WIDTH / 2 - 1 )
        Mix_PlayChannel(-1, pass_sound, 0);
        return true;
    }
    return false;
}
