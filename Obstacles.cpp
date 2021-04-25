#include"Obstacles.h"

Obstacles::Obstacles()
{
	sizer = { 0,0,0,0 };
	top = { SCREEN_WIDTH,0,0,0 }; bottom = { SCREEN_WIDTH,0,0,0 };
	increase_bottom = true; increase_top = true;
	type = 0;
	lowest_point_t = 0; highest_point_b = SCREEN_HEIGHT;
}

Obstacles::Obstacles(SDL_Renderer* renderer, int color)
{
	sizer = { 0,0,0,0 };
	top = { SCREEN_WIDTH,0,0,0 }; bottom = { SCREEN_WIDTH,0,0,0 };
	increase_bottom = true; increase_top = true;
	type = 0;
	lowest_point_t = 0; highest_point_b = SCREEN_HEIGHT;
	if (color == 1) loadImgFromFile("image/pipe_p.png", renderer);
	else loadImgFromFile("image/pipe_b.png", renderer);
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
	top.h = SCREEN_HEIGHT - DISTANCE_BETWEEN_BLOCK; bottom.h = SCREEN_HEIGHT - DISTANCE_BETWEEN_BLOCK;
	top.x = SCREEN_WIDTH; bottom.x = SCREEN_WIDTH;
	top.w = BLOCK_WIDTH; bottom.w = BLOCK_WIDTH;
	top.y = -rand() % top.h; bottom.y = top.y + top.h + DISTANCE_BETWEEN_BLOCK;

	if (type == 1 || type == 0) {
		lowest_point_t = rand() % (SCREEN_HEIGHT - DISTANCE_BETWEEN_BLOCK);
		highest_point_b = lowest_point_t + DISTANCE_BETWEEN_BLOCK;
	}
	else if (type == 2) 
	{
			increase_top = true; increase_bottom = false;
	}
	else if (type == 3) 
	{
		increase_top = false; increase_bottom = true;
	}
}

void Obstacles::setPosition(const int& x)
{
	top.x = x; bottom.x = x;
}

void Obstacles::move()
{
	top.x -= vel;
	bottom.x -= vel;
	if (type == 1) {
		if (increase_top) top.y++;
		else top.y--;
		if (top.y + top.h >= lowest_point_t) increase_top = false;
		if (top.y + top.h <= 0) increase_top = true;

		if (increase_bottom) bottom.y--;
		else bottom.y++;
		if (bottom.y <= highest_point_b) increase_bottom = false;
		if (bottom.y >= SCREEN_HEIGHT) increase_bottom = true;
	}
	else if (type == 2) {
		if (increase_top) {
			top.y++;
			bottom.y = top.y + top.h + DISTANCE_BETWEEN_BLOCK;
			increase_bottom = false;
			if (bottom.y == SCREEN_HEIGHT) {
				increase_bottom = true; increase_top = false;
			}
		}

		if (increase_bottom) {
			bottom.y--; 
			top.y = bottom.y - DISTANCE_BETWEEN_BLOCK - top.h;
			increase_top = false;
			if (top.y + top.h == 0) {
				increase_bottom = false; increase_top = true;
			}
		}
	}
	else if (type == 3) {
		if (increase_bottom) {
			bottom.y--;
			top.y = bottom.y - DISTANCE_BETWEEN_BLOCK - top.h;
			increase_top = false;
			if (top.y + top.h == 0) {
				increase_bottom = false; increase_top = true;
			}
		}

		if (increase_top) {
			top.y++;
			bottom.y = top.y + top.h + DISTANCE_BETWEEN_BLOCK;
			increase_bottom = false;
			if (bottom.y == SCREEN_HEIGHT) {
				increase_bottom = true; increase_top = false;
			}
		}
	}

	/*if (top.x + top.w <= 0) {
		top.x = SCREEN_WIDTH; bottom.x = SCREEN_WIDTH;
		int n = rand() % 4;
		setType(n);
		std::cout << n << std::endl;
	}*/
}
