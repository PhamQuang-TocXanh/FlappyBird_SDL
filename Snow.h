#pragma once
#include"Common.h"

class Snow : public Base
{
public:
	Snow();
	Snow(SDL_Renderer* renderer);
	~Snow();

	void render(SDL_Renderer* renderer);
	void setPosition(const int& x, const int& y);
	void setSize(const int& size);
	void fall();
	bool outScreen();
private:
	int x, y;
	double angle;
	SDL_Rect sizer;
};