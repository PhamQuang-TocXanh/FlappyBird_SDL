#include"Obstacles.h"

Obstacles::Obstacles()
{
	top = { 0,0,0,0 }; bottom = { 0,0,0,0 };
	increase_bottom = true; increase_top = true;
	type = 0;
}

Obstacles::Obstacles(SDL_Renderer* renderer)
{
	top = { 0,0,0,0 }; bottom = { 0,0,0,0 };
	increase_bottom = true; increase_top = true;
	type = 0;
	loadImgFromFile("image/pipe.png", renderer);
	sizer = getIMG();
	for (int i = 0; i < 2; i++) {
		src[i] = { 0,i * sizer.h / 2,sizer.w,sizer.h / 2 };
	}
}

Obstacles::~Obstacles()
{
	free();
}

void Obstacles::render(SDL_Renderer* renderer)
{
	Render(bottom.x, bottom.y, renderer, &src[1], &bottom);
	Render(top.x, top.y, renderer, &src[0], &top);
}

void Obstacles::setType(int type)
{
	this->type = type;

	top = { SCREEN_WIDTH,0,BLOCK_WIDTH,rand() % (SCREEN_HEIGHT - DISTANCE_BETWEEN_BLOCK) };
	bottom = { SCREEN_WIDTH,top.h + DISTANCE_BETWEEN_BLOCK,BLOCK_WIDTH,0 }; bottom.h = SCREEN_HEIGHT - bottom.y;
}

void Obstacles::setPosition(int x)
{
	top.x = x; bottom.x = x;
}

void Obstacles::move(SDL_Renderer* renderer)
{
	top.x -= 3;
	bottom.x -= 3;
	if (type == 1) {
		if (increase_top) top.y++;
		else top.y--;
		if (top.y >= 0) increase_top = false;
		if (top.y <= -top.h) increase_top = true;

		if (increase_bottom) bottom.y--;
		else bottom.y++;
		if (bottom.y <= SCREEN_HEIGHT - bottom.h) increase_bottom = false;
		if (bottom.y >= SCREEN_HEIGHT) increase_bottom = true;
	}
	else if (type == 2) {
		if (increase_top) {
			top.h++;
			bottom.y = top.y + top.h + DISTANCE_BETWEEN_BLOCK;
			bottom.h = SCREEN_HEIGHT - bottom.y;
			increase_bottom = false;
			if (bottom.y == SCREEN_HEIGHT) {
				increase_bottom = true; increase_top = false;
			}
		}

		if (increase_bottom) {
			bottom.y--; bottom.h++;
			top.y = bottom.y - DISTANCE_BETWEEN_BLOCK - top.h;
			increase_top = false;
			if (top.y + top.h == 0) {
				increase_bottom = false; increase_top = true;
			}
		}
	}
	else if (type == 3) {
		if (increase_bottom) {
			bottom.y--; bottom.h++;
			top.y = bottom.y - DISTANCE_BETWEEN_BLOCK - top.h;
			increase_top = false;
			if (top.y + top.h == 0) {
				increase_bottom = false; increase_top = true;
			}
		}

		if (increase_top) {
			top.h++;
			bottom.y = top.y + top.h + DISTANCE_BETWEEN_BLOCK;
			bottom.h = SCREEN_HEIGHT - bottom.y;
			increase_bottom = false;
			if (bottom.y == SCREEN_HEIGHT) {
				increase_bottom = true; increase_top = false;
			}
		}
	}

	if (top.x <= 0) {
		top.x = SCREEN_WIDTH; bottom.x = SCREEN_WIDTH;
		int n = rand() % 3 + 1;
		setType(n);
		std::cout << n << std::endl;
	}
}
