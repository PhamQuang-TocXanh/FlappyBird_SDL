#include "Common.h"
#include"Time.h"

using namespace std;

bool initSDL(SDL_Window*& window, SDL_Renderer*& renderer)
{
	if (SDL_Init(SDL_INIT_EVERYTHING | SDL_INIT_AUDIO) != 0) {
		logError_SDL(cerr, "Init SDL", true);
		return false;
	}
	else
	{
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
			cout << "Warning: linear texture filtering not enabble!\n";
		}
		window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE );
		if (window == NULL) {
			logError_SDL(cerr, "Window", true);
			return false;
		}
		else
		{
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED );
			if (renderer == NULL) {
				logError_SDL(cerr, "Renderer", true);
				return false;
			}
			else
			{
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
				int Flag = IMG_INIT_PNG;
				if (!(IMG_Init(Flag) & Flag)) {
					cerr << "Could not initialize SDL_image! SDL_image Error: " << IMG_GetError() << endl;
					return false;
				}

				if (TTF_Init() == -1) {
					cout << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << endl;
					return false;
				}

				if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
					cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << endl;
					return false;
				}
			}
		}
	}
	return true;
}

void closeSDL(SDL_Window* window, SDL_Renderer* renderer, Mix_Chunk* jump, Mix_Music* music, Mix_Chunk* pass, Mix_Chunk* gameOver, Mix_Music* bgmusic, Mix_Chunk* click, Mix_Chunk* breakHS)
{
	SDL_DestroyRenderer(renderer); renderer = NULL;
	SDL_DestroyWindow(window); window = NULL;
	Mix_FreeChunk(jump); jump = NULL;
	Mix_FreeChunk(pass); pass = NULL;
	Mix_FreeChunk(gameOver); gameOver = NULL;
	Mix_FreeChunk(click); click = NULL;
	Mix_FreeChunk(breakHS); breakHS = NULL;
	Mix_FreeMusic(bgmusic); bgmusic = NULL;
	Mix_FreeMusic(music); music = NULL;
	SDL_Quit();
	IMG_Quit();
	TTF_Quit();
	Mix_Quit();
}

void logError_SDL(std::ostream& out, std::string mes, bool fatal)
{
	out << mes << " Error: " << SDL_GetError() << endl;
	if (fatal) {
		SDL_Quit();
		exit(1);
	}
}

bool loadSound(Mix_Chunk*& jump, Mix_Music*& inGamemusic, Mix_Chunk*& pass, Mix_Chunk*& gameOver, Mix_Music*& bgmusic, Mix_Chunk*& click, Mix_Chunk*& breakHS)
{
	bool loadOkay = true;
	jump = Mix_LoadWAV("sound/jum_ascend.wav");
	if (jump == NULL) {
		cerr << "Unable to load jump sound effect! SDL_mixer Error: " << Mix_GetError() << endl;
		loadOkay = false;
	}
	inGamemusic = Mix_LoadMUS("sound/ms.wav");
	if (inGamemusic == NULL) {
		cerr << "Unable to load music! SDL_mixer Error: " << Mix_GetError() << endl;
		loadOkay = false;
	}
	pass = Mix_LoadWAV("sound/pass_sound.wav");
	if (pass == NULL) {
		cerr << "Unable to load pass sound effect! SDL_mixer Error: " << Mix_GetError() << endl;
		loadOkay = false;
	}
	gameOver = Mix_LoadWAV("sound/gameover.wav");
	if (gameOver == NULL) {
		cerr << "Unable to load game over sound effect! SDL_mixer Error: " << Mix_GetError() << endl;
		loadOkay = false;
	}
	bgmusic = Mix_LoadMUS("sound/menu_music.wav");
	if (bgmusic == NULL) {
		cerr << "Unable to load background music in menu! SDL_mixer Error: " << Mix_GetError() << endl;
		loadOkay = false;
	}
	click = Mix_LoadWAV("sound/click.wav");
	if (click == NULL) {
		cerr << "Unable to load click sound effect! SDL_mixer Error: " << Mix_GetError() << endl;
		loadOkay = false;
	}
	breakHS = Mix_LoadWAV("sound/breakHighScore.wav");
	if (breakHS == NULL) {
		cerr << "Unable to load BreakHighScore sound effect! SDL_mixer Error: " << Mix_GetError() << endl;
		loadOkay = false;
	}
	return loadOkay;
}

bool checkCollision(SDL_Rect collider, SDL_Rect top, SDL_Rect bottom)
{
	if (collider.x + collider.w < top.x || collider.x + collider.w < bottom.x) return false;
	if (collider.x > top.x + top.w || collider.x > bottom.x + bottom.w) return false;
	if (collider.y + collider.h - 4 < bottom.y && collider.y + 10 > top.y + top.h) return false;

	return true;
}

Base::Base()
{
	object = NULL;
	object_img.x = 0; object_img.y = 0;
}

Base::~Base()
{
	free();
}

void Base::loadImgFromFile(const std::string path, SDL_Renderer*& renderer)
{
	free();
	SDL_Texture* img = NULL;
	SDL_Surface * loadedIMG = IMG_Load(path.c_str());
	if (loadedIMG == NULL) {
		cerr << "Failed to load image " << path << " SDL_image Error: " << IMG_GetError() << endl;
		return;
	}
	else
	{
		SDL_SetColorKey(loadedIMG, 1, SDL_MapRGB(loadedIMG->format, 255, 255, 255));
		img = SDL_CreateTextureFromSurface(renderer, loadedIMG);
		if (img == NULL) {
			logError_SDL(cerr, "Unable to create texture from surface!", true);
		}
		else
		{
			object_img.w = loadedIMG->w; object_img.h = loadedIMG->h;
		}
	}
	SDL_FreeSurface(loadedIMG);
	object = img;
}

void Base::loadFromRenderedText(SDL_Renderer* renderer, const std::string& text, const std::string& path, TTF_Font* font, int size)
{
	free();
	font = TTF_OpenFont(path.c_str(), size);
	if (font == NULL) {
		cerr << "Failed to load font! SDL_ttf Error: " << TTF_GetError() << endl;
		return;
	}
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
	if (textSurface == NULL) {
		cerr << "Failed to render text surface! SDL_ttf Error: " << TTF_GetError() << endl;
		return;
	}
	else
	{
		object = SDL_CreateTextureFromSurface(renderer, textSurface);
		if (object == NULL) {
			cerr << "Unable to create texture from rendered text! Error: " << SDL_GetError() << endl;
			return;
		}
		else
		{
			object_img.w = textSurface->w; object_img.h = textSurface->h;
		}
	}
	SDL_FreeSurface(textSurface);
}

void Base::Render(const double& x, const double& y, SDL_Renderer*& renderer, SDL_Rect* clip, SDL_Rect* des, double angle, SDL_RendererFlip flip)
{
	object_img.x = x; object_img.y = y;
	SDL_Rect renderQuad = { x,y,object_img.w,object_img.h };
	if (des != NULL) {
		renderQuad.w = des->w; renderQuad.h = des->h;
	}
	SDL_RenderCopyEx(renderer, object, clip, &renderQuad, angle, NULL, flip);
}

void Base::free()
{
	if (object != NULL) {
		SDL_DestroyTexture(object);
		object = NULL;
		object_img.w = 0;
		object_img.h = 0;
	}
}