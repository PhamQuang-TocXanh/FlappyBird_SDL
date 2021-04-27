#include"Common.h"
#include"Time.h"
#include"MainObject.h"
#include"Button.h"
#include"Obstacles.h"
#include"Snow.h"
using namespace std;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

TTF_Font* font = NULL;

Mix_Chunk* jump_sound = NULL;
Mix_Music* bgmusic = NULL;
Mix_Music* music = NULL;
Mix_Chunk* pass_sound = NULL;
Mix_Chunk* gameOver = NULL;
Mix_Chunk* click = NULL;
Mix_Chunk* breakHighScore = NULL;

SDL_Rect screen_size = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

void HandlePlayButton(Button& play, SDL_Event* e, bool& quit_menu);
void HandleExitButton(Button& exit, SDL_Event* e, bool& quit_game, bool& quit_menu);
void HandleHelpButton(Button& help, SDL_Event* e, Button& back, Base& instruction, bool& quit_game, bool& quit_menu);
void HandlePlayAgain(Button& again, SDL_Event* e, Button& exit, bool& inGame, bool& playAgain, bool& quit_game, int score = 0, bool newHighScore = false);
void HandleChooseScreen(SDL_Event* e, int& backGround_type, bool& quit_game, Button& back);

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

        Base Scoreb, Scoref; Scoref.setTextColor(45, 251, 199);
        stringstream scoreText;
        int score = 0;

        Base highScore;
        stringstream highScoreText;
        int high_score = 0;

        SDL_Rect window_size = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
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
        back.setPosition(0, 0); back.setSize(80, 75);

        bool quit_game = false;
        bool play_game = false;
        bool help_menu = false;

        if (!loadSound(jump_sound, music, pass_sound, gameOver, bgmusic, click, breakHighScore)) {
            cerr << "failed to load sound!\n";
        }
        
        SDL_Event e;

        while (!quit_game) {
            bool quit_menu = false;

            if (Mix_PlayingMusic() == 0)
            {
                Mix_PlayMusic(bgmusic, -1);
            }
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

            Base press_start; press_start.loadImgFromFile("image/press_to_start.png", renderer);
            int bird_type = 1, backGround_type = 1;

            HandleChooseScreen(&e, backGround_type, quit_game, back);
            Mix_HaltMusic();
            if (quit_game) continue;

            Base bg, bg1, bg2, bg3, bg4, bg5;
            if (backGround_type == 1) {
                bg.loadImgFromFile("image/pink.jpg", renderer);
                bg1.loadImgFromFile("image/bg1.png", renderer);
                bg2.loadImgFromFile("image/bg2.png", renderer);
                bg3.loadImgFromFile("image/bg3.png", renderer);
                bg4.loadImgFromFile("image/bg4.png", renderer);
                bg5.loadImgFromFile("image/bg5.png", renderer);
            }
            else if (backGround_type == 2) {
                bg.loadImgFromFile("image/bluewhite1.jpg", renderer);
                bg1.loadImgFromFile("image/11.png", renderer);
                bg2.loadImgFromFile("image/22.png", renderer);
                bg3.loadImgFromFile("image/33.png", renderer);
                bg4.loadImgFromFile("image/44.png", renderer);
                bg5.loadImgFromFile("image/55.png", renderer);
            }
            double scroll1 = 0, scroll2 = 0, scroll3 = 0, scroll4 = 0;

            bool playAgain = true;
            while (playAgain)
            {
                score = 0;
                Bird flappy(renderer, bird_type); 

                ifstream in;
                in.open("HighScore.txt");
                in >> high_score;
                in.close();
                highScoreText.str("");
                highScoreText << "High Score: " << high_score << endl;
                highScore.loadFromRenderedText(renderer, highScoreText.str().c_str(), "k.ttf", font);

                vector<Obstacles*> ob;
                Obstacles* p = NULL;
                for (int i = 0; i < SCREEN_WIDTH / (BLOCK_WIDTH + 170); i++) {
                    p = new Obstacles(renderer, backGround_type);
                    p->setType(rand() % 4);
                    ob.push_back(p);
                    if (i != 0) ob.at(i)->setPosition(ob.at(i - 1)->getPosition() + BLOCK_WIDTH + 170);
                }
                /*vector<Obstacles*> cnv;
                Obstacles* s = nullptr, *ss;
                s = new Obstacles(renderer, backGround_type); s->setType(rand() % 4);
                cnv.push_back(s);*/

                vector<Snow*> snow;
                Snow* sn = NULL;
                for (int i = 0; i < 40; i++) {
                    sn = new Snow(renderer);
                    sn->setPosition(rand() % (SCREEN_WIDTH * 2 - 10) + 10, -rand() % (SCREEN_HEIGHT / 2));
                    sn->setSize(SNOW_SIZE + rand() % SNOW_SIZE * 2);
                    snow.push_back(sn);
                }

                bool inGame = true, press_to_start = true, newHighScore = false, rangeScore = true;
                time.start();
                while (inGame) {
                    if (Mix_PlayingMusic() == 0) Mix_PlayMusic(music, -1);
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
                        if (e.type == SDL_KEYDOWN || e.type == SDL_MOUSEBUTTONDOWN) {
                            press_to_start = false;
                        }
                    }

                    /*timeText.str("");
                    timeText << "Time: " << time.getTicks() / 1000 << endl;
                    Time.loadFromRenderedText(renderer, timeText.str().c_str(), "k.ttf", font);*/
                    
                    scoreText.str("");
                    scoreText << score << endl;
                    Scoreb.loadFromRenderedText(renderer, scoreText.str().c_str(), "k.ttf", font, 60);
                    Scoref.loadFromRenderedText(renderer, scoreText.str().c_str(), "k.ttf", font, 60);

                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    SDL_RenderClear(renderer);

                    bg.Render(0, 0, renderer, NULL, &window_size);
                    bg1.Render(0, 0, renderer, NULL, &window_size);
                    bg2.Render(scroll1, 0, renderer, NULL, &window_size);
                    bg2.Render(scroll1 + SCREEN_WIDTH, 0, renderer, NULL, &window_size);
                    bg3.Render(scroll2, 0, renderer, NULL, &window_size);
                    bg3.Render(scroll2 + SCREEN_WIDTH, 0, renderer, NULL, &window_size);
                    bg4.Render(scroll3, 0, renderer, NULL, &window_size);
                    bg4.Render(scroll3 + SCREEN_WIDTH, 0, renderer, NULL, &window_size);
                    bg5.Render(scroll4, 0, renderer, NULL, &window_size);
                    bg5.Render(scroll4 + SCREEN_WIDTH, 0, renderer, NULL, &window_size);
                    scroll1 -= 0.2;
                    if (scroll1 < -SCREEN_WIDTH) scroll1 = 0;
                    scroll2 -= 0.4;
                    if (scroll2 < -SCREEN_WIDTH) scroll2 = 0;
                    scroll3 -= 0.7;
                    if (scroll3 < -SCREEN_WIDTH) scroll3 = 0;
                    scroll4 -= 1.1;
                    if(scroll4 < -SCREEN_WIDTH) scroll4 = 0;
                             
                    int nearest = SCREEN_WIDTH;
                    for (int i = 0; i < ob.size(); i++) {
                        if (flappy.Pass_Obstacle(ob.at(i), pass_sound) == true && rangeScore == true) {
                            score++;
                            rangeScore = false;
                        }
                        if (ob.at(i)->getPosition() + BLOCK_WIDTH < nearest) {
                            nearest = ob.at(i)->getPosition() + BLOCK_WIDTH;
                        }
                    }
                    if (BIRD_WIDTH >= nearest) rangeScore = true;
                    /*for (int i = 0; i < cnv.size(); i++) {
                        if (flappy.Pass_Obstacle(cnv.at(i), pass_sound) == true) score++;
                    }*/
                    if (score > high_score) {
                        newHighScore = true;
                        ofstream out("HighScore.txt");
                        out << score << endl;
                        out.close();
                    } 

                    flappy.render(renderer);

                    if(press_to_start==true) press_start.Render(0, 0, renderer, NULL, &window_size);                    
                    else
                    {
                        for (vector<Obstacles*>::iterator i = ob.begin(); i != ob.end(); i++) {
                            (*i)->render(renderer);
                            (*i)->move();
                        }
                        for (int i = 0; i < ob.size(); i++) {
                            if (ob.at(i)->getPosition() + BLOCK_WIDTH <= 0) {
                                int r = rand() % 4; cout << "type: " << r << " at: " << i << endl;
                                ob.at(i)->setType(r);
                                if(i!=0) ob.at(i)->setPosition(ob.at(i - 1)->getPosition() + BLOCK_WIDTH + 170);
                                else
                                {
                                    ob.at(0)->setPosition(ob.at(ob.size() - 1)->getPosition() + BLOCK_WIDTH + 170);
                                }
                            }
                        }
                        /*for (vector<Obstacles*>::iterator i = cnv.begin(); i != cnv.end(); i++) {
                            (*i)->render(renderer);
                            (*i)->move();
                        }cout << cnv.size() - 1 << endl;
                        if (cnv.at(cnv.size() - 1)->getPosition() <= SCREEN_WIDTH - 170 - BLOCK_WIDTH) {
                            ss = new Obstacles(renderer, backGround_type); ss->setType(rand() % 4);
                            cnv.push_back(ss);
                        }cout << cnv.size() - 1 << endl;
                        if (cnv.at(0)->getPosition() + BLOCK_WIDTH <= 0) {
                            delete cnv.at(0);
                            cnv.erase(cnv.begin());
                        }*/

                        if (flappy.dead == false)
                        {
                            flappy.move(ob);
                            //flappy.move(cnv);
                            SDL_Rect score_size = Scoreb.getIMG(), highscore_size=highScore.getIMG();
                            Scoreb.Render(SCREEN_WIDTH / 2 - score_size.w / 2, 100, renderer);
                            Scoref.Render(SCREEN_WIDTH / 2 - score_size.w / 2 - 3, 100 - 3, renderer);
                            /*Time.Render(SCREEN_WIDTH - highscore_size.w - 10, highscore_size.h, renderer);*/
                            highScore.Render(SCREEN_WIDTH - highscore_size.w - 10, 0, renderer);
                        }
                        else
                        {
                            time.pause();
                            Mix_HaltMusic();
                            SDL_Delay(100);
                            HandlePlayAgain(again, &e, exit2, inGame, playAgain, quit_game, score, newHighScore);
                        }
                    }       

                    for (int i = 0; i < snow.size(); i++) {
                        snow.at(i)->render(renderer);
                        snow.at(i)->fall();
                        if (snow.at(i)->outScreen()) {
                            snow.at(i)->setPosition(rand() % (SCREEN_WIDTH * 2 - 10) + 10, -rand() % (SCREEN_HEIGHT / 2)); 
                            sn->setSize(SNOW_SIZE + rand() % SNOW_SIZE * 2);
                        }
                    }

                    SDL_RenderPresent(renderer);
                    countedFrames++;
                    int frametick = capTimer.getTicks();
                    if (frametick < SCREEN_TICK_PER_FRAME) SDL_Delay(SCREEN_TICK_PER_FRAME - frametick);
                }

                for (vector<Obstacles*>::iterator i = ob.begin(); i != ob.end(); i++) {
                    delete* i;
                } ob.clear();
                for (vector<Snow*>::iterator i = snow.begin(); i != snow.end(); i++) {
                    delete* i;
                } snow.clear();
                /*for (vector<Obstacles*>::iterator i = cnv.begin(); i != cnv.end(); i++) {
                    delete* i;
                } cnv.clear();*/
                time.stop();
                newHighScore = false;
                Mix_HaltMusic();
            }
        }
    }
    closeSDL(window, renderer, jump_sound, music, pass_sound, gameOver, bgmusic, click, breakHighScore);
    return 0;
}



void HandlePlayButton(Button& play, SDL_Event* e, bool& quit_menu) {
    play.handleEvent(e);
    if (play.inside) {
        if (e->type == SDL_MOUSEBUTTONDOWN) {
            Mix_PlayChannel(-1, click, 0);
            quit_menu = true;
        }
    }
}
void HandleExitButton(Button& exit, SDL_Event* e, bool& quit_game, bool& quit_menu) {
    exit.handleEvent(e);
    if (exit.inside) {
        if (e->type == SDL_MOUSEBUTTONDOWN) {
            Mix_PlayChannel(-1, click, 0);
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
            Mix_PlayChannel(-1, click, 0);
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
                    if (back.inside && e->type == SDL_MOUSEBUTTONDOWN) {
                        help_menu = false; Mix_PlayChannel(-1, click, 0);
                    }
                }
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderClear(renderer);
                instruction.Render(0, 0, renderer, NULL, &screen_size);
                back.render(renderer);

                SDL_RenderPresent(renderer);
            }
        }
    }
}

void HandlePlayAgain(Button& again, SDL_Event* e, Button& exit2, bool& inGame, bool& playAgain, bool& quit_game, int score, bool newHighScore) {
    Base Score; Base SCore;
    stringstream scoreText;
    scoreText.str("");
    if (newHighScore) {
        Mix_PlayChannel(-1, breakHighScore, 0);
        scoreText << "NEW HIGH SCORE: " << score << endl;
        Score.setTextColor(45, 123, 210);
        Score.loadFromRenderedText(renderer, scoreText.str().c_str(), "k.ttf", font, 45);
        SCore.setTextColor(245, 247, 56);
        SCore.loadFromRenderedText(renderer, scoreText.str().c_str(), "k.ttf", font, 45);
    }
    else
    {
        Mix_PlayChannel(-1, gameOver, 0);
        scoreText << "SCORE: " << score << endl;
        Score.loadFromRenderedText(renderer, scoreText.str().c_str(), "k.ttf", font, 35);
        SCore.setTextColor(41, 113, 162);
        SCore.loadFromRenderedText(renderer, scoreText.str().c_str(), "k.ttf", font, 35);
    }
    SDL_Rect score_size = Score.getIMG();

    again.setPosition(SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2, SCREEN_HEIGHT / 2);
    exit2.setPosition(SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2, SCREEN_HEIGHT / 2 + 20 + BUTTON_HEIGHT);

    bool playAgainScreen = true;

    while (playAgainScreen) {
        while (SDL_PollEvent(e) != 0)
        {
            if (e->type == SDL_QUIT) {
                playAgainScreen = false;
                inGame = false;
                playAgain = false;
                quit_game = true;
            }
            again.handleEvent(e);
            exit2.handleEvent(e);
        }
        again.render(renderer);
        exit2.render(renderer);
        if (exit2.inside && e->type == SDL_MOUSEBUTTONDOWN) {
            Mix_PlayChannel(-1, click, 0);
            playAgainScreen = false;
            inGame = false;
            playAgain = false;
        }
        if (again.inside && e->type == SDL_MOUSEBUTTONDOWN) {
            Mix_PlayChannel(-1, click, 0);
            playAgainScreen = false;
            inGame = false;
        }
        Score.Render(SCREEN_WIDTH / 2 - score_size.w / 2, again.getPositionY() - score_size.h - 10, renderer);
        SCore.Render(SCREEN_WIDTH / 2 - score_size.w / 2 - 3, again.getPositionY() - score_size.h - 10-3, renderer);
        SDL_RenderPresent(renderer);
    }
}

void HandleChooseScreen(SDL_Event* e, int& backGround_type, bool& quit_game, Button& back)
{
    Button type1(renderer, BUTTON_MAP_1);
    Button type2(renderer, BUTTON_MAP_2);
    type1.setPosition(SCREEN_WIDTH / 12, SCREEN_HEIGHT / 3); type2.setPosition(SCREEN_WIDTH * 2 / 3, SCREEN_HEIGHT / 3);
    type1.setSize(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 3); type2.setSize(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 3);

    Base chooseScreen; chooseScreen.loadImgFromFile("image/choose.png", renderer);

    bool screen = true;
    while (screen) {
        while (SDL_PollEvent(e) != 0)
        {
            if (e->type == SDL_QUIT) {
                screen = false;
                quit_game = true;
            }
            type1.handleEvent(e);
            type2.handleEvent(e);
        }
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        chooseScreen.Render(0, 0, renderer, NULL, &screen_size);
        type1.render(renderer);
        type2.render(renderer);

        if (type1.inside && e->type == SDL_MOUSEBUTTONDOWN) {
            Mix_PlayChannel(-1, click, 0);
            backGround_type = 1;
            screen = false;
        }
        if (type2.inside && e->type == SDL_MOUSEBUTTONDOWN) {
            Mix_PlayChannel(-1, click, 0);
            backGround_type = 2;
            screen = false;
        }

        SDL_RenderPresent(renderer);
    }

}