#include"Snow.h"

Snow::Snow()
{
	x = SCREEN_WIDTH;
	y = 0;
	angle = 0;
	sizer = { 0,0,SNOW_SIZE,SNOW_SIZE };
}

Snow::Snow(SDL_Renderer* renderer)
{
	x = SCREEN_WIDTH;
	y = 0;
	angle = 0;
	int r = rand() % 2;
	if (r == 1) loadImgFromFile("image/snow.png", renderer);
	else loadImgFromFile("image/snowflakes1.png", renderer);
	sizer = { 0,0,SNOW_SIZE,SNOW_SIZE };
}

Snow::~Snow()
{
	free();
}

void Snow::render(SDL_Renderer* renderer)
{
	angle--;
	if (angle <= -180) angle = 0;
	Render(x, y, renderer, NULL, &sizer, angle);
}

void Snow::setPosition(const int& x, const int& y)
{
	this->x = x; this->y = y;
}

void Snow::setSize(const int& size)
{
	sizer.w = size; sizer.h = size;
}

void Snow::fall()
{
	x -= 2; y++;
}

bool Snow::outScreen()
{
	if (x + sizer.w <= 0 || y >= SCREEN_HEIGHT) return true;
	return false;
}
