#pragma once
#include"Common.h"
#include"Obstacles.h"
const int velocity = 10;

class Bird : public Base
{
public:
	Bird();
	Bird(SDL_Renderer* renderer, int num = 1);
	~Bird();
	bool dead = false;

	void handleEvent(SDL_Event& e, Mix_Chunk* jump_sound = NULL);
	void move(std::vector<Obstacles*> ob);
	void render(SDL_Renderer* renderer);
	void Gravity();
	bool Pass_Obstacle(Obstacles obstacle, Mix_Chunk* pass_sound = NULL);
	bool Pass_Obstacle(Obstacles* obstacle, Mix_Chunk* pass_sound = NULL);
	SDL_Rect getCollider() { return collider; };
	void setPosition(const int& y) { collider.y = y; };
	int getPosition() { return collider.y; };
private:
	int pos_x, pos_y;
	int vel_x, vel_y;
	SDL_Rect collider;
	SDL_Rect animation[4];
	int frame = 0, angle = 0;
	double gravity = 1;
	bool jump = false, inScoreRange = false;
};
