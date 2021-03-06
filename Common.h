#pragma once
#include<iostream>
#include<SDL_image.h>
#include<SDL.h>
#include<SDL_ttf.h>
#include<SDL_mixer.h>
#include<cstring>
#include<ctime>
#include<string>
#include<vector>
#include<sstream>
#include<cstdlib>
#include<fstream>

const int FPS = 65;
const int SCREEN_TICK_PER_FRAME = 1000 / FPS;
const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 600;
const std::string title = "QuangDepTrai";
const int BUTTON_WIDTH = 150;
const int BUTTON_HEIGHT = 100;
const int BIRD_WIDTH = 64;
const int BIRD_HEIGHT = 48;
const int BLOCK_WIDTH = 80;
const int DISTANCE_BETWEEN_BLOCK = 150;
const int SNOW_SIZE = 10;


bool initSDL(SDL_Window*& window, SDL_Renderer*& renderer);
void closeSDL(SDL_Window* window, SDL_Renderer* renderer, Mix_Chunk* jump, Mix_Music* music, Mix_Chunk* pass, Mix_Chunk* gameOver, Mix_Music* bgmusic, Mix_Chunk* click, Mix_Chunk* breakHS);
void logError_SDL(std::ostream& out, std::string mes, bool fatal = false);
bool loadSound(Mix_Chunk*& jump, Mix_Music*& music, Mix_Chunk*& pass, Mix_Chunk*& gameOver, Mix_Music*& bgmusic, Mix_Chunk*& click, Mix_Chunk*& breakHS);
bool checkCollision(SDL_Rect collider, SDL_Rect top, SDL_Rect bottom);

enum ButtonSprite {
	BUTTON_SPRITE_MOUSE_OUT,
	BUTTON_SPRITE_MOUSE_OVER_MOTION,
	BUTTON_SPRITE_TOTAL
};

enum ChooseButton {
	BUTTON_PLAY,
	BUTTON_EXIT,
	BUTTON_HELP,
	BUTTON_BACK,
	BUTTON_PLAY_AGAIN,
	BUTTON_MAP_1,
	BUTTON_MAP_2,
	BUTTON_TOTAL
};

class Base
{
public:
	Base();
	~Base();
	void loadImgFromFile(const std::string path, SDL_Renderer*& renderer);
	SDL_Rect getIMG() { return object_img; };
	void loadFromRenderedText(SDL_Renderer* renderer, const std::string& text, const std::string& path, TTF_Font* font, int size = 18);
	void Render(const double& x, const double& y, SDL_Renderer*& renderer, SDL_Rect* clip = NULL, SDL_Rect* des = NULL, double angle = 0, SDL_RendererFlip flip = SDL_FLIP_NONE);
	void setTextColor(Uint8 r, Uint8 g, Uint8 b) { color = { r,g,b,255 }; };
	void free();
private:
	SDL_Texture* object;
	SDL_Rect object_img;
	SDL_Color color = { 0,0,0,0 };
};
