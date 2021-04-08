#pragma once
#include"Common.h"

class Timer
{
public:
	Timer();
	~Timer();

	void start();
	void stop();
	void pause();
	void unpause();

	Uint32 getTicks();
	bool isStarted();
	bool isPaused();
private:
	Uint32 StartTicks;
	Uint32 PausedTicks;
	bool Paused;
	bool Started;
};