#pragma once
#include"Common.h"

class Obstacles : public Base
{
public:
	Obstacles();
	Obstacles(SDL_Renderer* renderer, int color = 1);
	~Obstacles();
	
	void render(SDL_Renderer* renderer);
	SDL_Rect getTopBlock() { return top; };
	SDL_Rect getBottomBlock() { return bottom; };
	void setType(int type = 0);
	void setPosition(const int& x);
	int getPosition() { return top.x; };
	void setVelocity(const int& vel) { this->vel = vel; };
	void move();
private:
	SDL_Rect top, bottom;
	SDL_Rect sizer, src[2];
	bool increase_top, increase_bottom;
	int type, vel = 4;
	int lowest_point_t, highest_point_b;
};
