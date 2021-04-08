#include"Common.h"
#include"Time.h"
#include"MainObject.h"
#include"Button.h"
#include"Obstacles.h"
using namespace std;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

TTF_Font* font = NULL;

Mix_Chunk* jump_sound = NULL;
Mix_Music* music = NULL;
Mix_Chunk* pass_sound = NULL;

SDL_Rect screen_size = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

void HandlePlayButton(Button& play, SDL_Event* e, bool& quit_menu);
void HandleExitButton(Button& exit, SDL_Event* e, bool& quit_game, bool& quit_menu);
void HandleHelpButton(Button& help, SDL_Event* e, Button& back, Base& instruction, bool& quit_game, bool& quit_menu);
void HandlePlayAgain(Button& again, SDL_Event* e, Button& exit, bool& inGame, bool& playAgain, bool& quit_game);


int main(int argc, char* argv[])
{
    srand(time(0));
    if (!initSDL(window, renderer)) {
        cerr << "Failed to initialize!\n";
    }
    else
    {
        Base Time;
        Timer time; 
        stringstream timeText;
        Timer capTimer;
        int countedFrames = 0;

        Base Score;
        stringstream scoreText;
        int score = 0;

        Base highScore;
        stringstream highScoreText;
        int high_score;

        Base backgound; backgound.loadImgFromFile("image/background.png", renderer);
        SDL_Rect background_rect = backgound.getIMG();
        Base menu; menu.loadImgFromFile("image/menu.png", renderer);
        Base instruction; instruction.loadImgFromFile("image/INSTRUCTION.png", renderer);

        Button play(renderer, BUTTON_PLAY);
        Button exit1(renderer, BUTTON_EXIT);
        Button help(renderer, BUTTON_HELP);
        Button back(renderer, BUTTON_BACK);
        Button again(renderer, BUTTON_PLAY_AGAIN);
        Button exit2(renderer, BUTTON_EXIT);

        play.setPosition(SCREEN_WIDTH / 2 - BUTTON_WIDTH * 3 / 2 - 50, 450);
        help.setPosition(SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2, 450);
        exit1.setPosition(SCREEN_WIDTH / 2 + BUTTON_WIDTH / 2 + 50, 450);
        back.setPosition(0, 0);

        bool quit_game = false;
        bool play_game = false;
        bool help_menu = false;

        if (!loadSound(jump_sound, music, pass_sound)) {
            cerr << "failed to load sound!\n";
        }

        int scrolling = 0;
        
        SDL_Event e;

        while (!quit_game) {
            bool quit_menu = false;

            while (!quit_menu)
            {
                while (SDL_PollEvent(&e) != 0)
                {
                    if (e.type == SDL_QUIT) {
                        quit_game = true;
                        quit_menu = true;
                    }
                    HandlePlayButton(play, &e, quit_menu);
                    HandleExitButton(exit1, &e, quit_game, quit_menu);
                    HandleHelpButton(help, &e, back, instruction, quit_game, quit_menu);
                }
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderClear(renderer);

                menu.Render(0, 0, renderer, NULL, &screen_size);
                play.render(renderer);
                help.render(renderer);
                exit1.render(renderer);

                SDL_RenderPresent(renderer);
            }
            if (quit_game) continue;

            bool playAgain = true;
            while (playAgain)
            {
                time.start();
                score = 0;
                Bird flappy(renderer);

                Obstacles b(renderer);
                b.setType(rand() % 3 + 1);
                vector<Obstacles> o;//dung vector chua render duoc
                for (int i = 0; i < SCREEN_WIDTH / (BLOCK_WIDTH + 120); i++) {
                    Obstacles n(renderer); n.setType(rand() % 3 + 1);
                    if (i != 0) {
                        n.setPosition(o.at(i - 1).getPosition() + BLOCK_WIDTH + 120);
                    }
                    o.push_back(n);
                }
                
                ifstream inF("HighScore.txt");

                bool inGame = true;
                while (inGame) {
                    if (Mix_PlayingMusic() == 0)
                    {
                        Mix_PlayMusic(music, -1);
                    }
                    capTimer.start();
                    while (SDL_PollEvent(&e) != 0)
                    {
                        if (e.type == SDL_QUIT) {
                            inGame = false;
                            playAgain = false;
                            quit_game = true;
                        }
                        else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_p) {
                            if (Mix_PausedMusic() == 1) Mix_ResumeMusic();
                            else Mix_PauseMusic();
                        }
                        flappy.handleEvent(e, jump_sound);
                    }
                    --scrolling;
                    if (scrolling < -background_rect.w) scrolling = 0;

                    timeText.str("");
                    timeText << "Time: " << time.getTicks() / 1000 << endl;
                    Time.loadFromRenderedText(renderer, timeText.str().c_str(), "k.ttf", font);

                    scoreText.str("");
                    scoreText << "Score: " << score << endl;
                    Score.loadFromRenderedText(renderer, scoreText.str().c_str(), "k.ttf", font);

                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    SDL_RenderClear(renderer);

                    backgound.Render(scrolling, 0, renderer, NULL);
                    backgound.Render(scrolling + background_rect.w, 0, renderer, NULL);

                    SDL_Rect score_size = Score.getIMG();
                    Score.Render(SCREEN_WIDTH - score_size.w, 0, renderer);
                    Time.Render(SCREEN_WIDTH - score_size.w, score_size.h, renderer);

                    flappy.Gravity();
                    //flappy.move(b);
                    flappy.move(o);
                    
                    //if (flappy.Pass_Obstacle(b,pass_sound)) score++;
                    for (int i = 0; i < SCREEN_WIDTH / (BLOCK_WIDTH + 120); i++) {
                        if (flappy.Pass_Obstacle(o.at(i), pass_sound)) score ++;
                    }

                    //b.render(renderer);
                    //b.move(renderer);
                    for (int i = 0; i < SCREEN_WIDTH / (BLOCK_WIDTH + 120); i++) {
                        o.at(i).render(renderer);
                        o.at(i).move(renderer);
                    }

                    flappy.render(renderer); 
                    if (flappy.dead) {
                        time.pause();
                        Mix_HaltMusic();
                        HandlePlayAgain(again, &e, exit2, inGame, playAgain, quit_game);
                    }

                    SDL_RenderPresent(renderer);
                    countedFrames++;
                    int frametick = capTimer.getTicks();
                    if (frametick < SCREEN_TICK_PER_FRAME) SDL_Delay(SCREEN_TICK_PER_FRAME - frametick);
                }    
            }
            time.stop();
        }
        Time.free();
    }
    closeSDL(window, renderer, jump_sound, music);
    return 0;
}



void HandlePlayButton(Button& play, SDL_Event* e, bool& quit_menu) {
    play.handleEvent(e);
    if (play.inside) {
        if (e->type == SDL_MOUSEBUTTONDOWN) {
            quit_menu = true;
        }
    }
}
void HandleExitButton(Button& exit, SDL_Event* e, bool& quit_game, bool& quit_menu) {
    exit.handleEvent(e);
    if (exit.inside) {
        if (e->type == SDL_MOUSEBUTTONDOWN) {
            quit_menu = true;
            quit_game = true;
        }
    }
}
void HandleHelpButton(Button& help, SDL_Event* e, Button& back, Base& instruction, bool& quit_game, bool& quit_menu) {
    help.handleEvent(e);
    bool help_menu = true;
    if (help.inside) {
        if (e->type == SDL_MOUSEBUTTONDOWN) {
            while (help_menu)
            {
                while (SDL_PollEvent(e) != 0)
                {
                    if (e->type == SDL_QUIT) {
                        quit_menu = true;
                        quit_game = true;
                        help_menu = false;
                    }
                    back.handleEvent(e);
                    if (back.inside && e->type == SDL_MOUSEBUTTONDOWN) help_menu = false;
                }
                instruction.Render(0, 0, renderer, NULL, &screen_size);
                back.render(renderer);

                SDL_RenderPresent(renderer);
            }
        }
    }
}

void HandlePlayAgain(Button& again, SDL_Event* e, Button& exit, bool& inGame, bool& playAgain, bool& quit_game) {
    again.setPosition(SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2, SCREEN_HEIGHT / 2);
    exit.setPosition(SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2, SCREEN_HEIGHT / 2 + 20 + BUTTON_HEIGHT);
    bool playAgainScreen = true;
    while (playAgainScreen)
    {
        while (SDL_PollEvent(e) != 0)
        {
            if (e->type == SDL_QUIT) {
                playAgainScreen = false;
                inGame = false;
                playAgain = false;
                quit_game = true;
            }
            again.handleEvent(e);
            exit.handleEvent(e);
        }
        again.render(renderer);
        exit.render(renderer);
        if (exit.inside && e->type == SDL_MOUSEBUTTONDOWN) {
            playAgainScreen = false;
            inGame = false;
            playAgain = false;
        }
        if (again.inside && e->type == SDL_MOUSEBUTTONDOWN) {
            playAgainScreen = false;
            inGame = false;
        }

        SDL_RenderPresent(renderer);
    }
}