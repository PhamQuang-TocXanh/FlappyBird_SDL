#pragma once
#include"Common.h"
const int BLOCK_WIDTH = 80;
const int DISTANCE_BETWEEN_BLOCK = 300;

class Obstacles : public Base
{
public:
	Obstacles();
	Obstacles(SDL_Renderer* renderer);
	~Obstacles();
	
	void render(SDL_Renderer* renderer);
	SDL_Rect getTopBlock() { return top; };
	SDL_Rect getBottomBlock() { return bottom; };
	void setType(int type = 0);
	void setPosition(int x);
	int getPosition() { return top.x; };
	void move(SDL_Renderer* renderer);
private:
	SDL_Rect top, bottom;
	SDL_Rect sizer, src[2];
	bool increase_top, increase_bottom;
	int type;
};
