#pragma once

#include <iostream>

class GameStatistics {
public:
	void printStartingMessage();

	static GameStatistics* getGameStatistics();

	void update();

	void printEndingMessage();

	unsigned int getScore();

	~GameStatistics();
private:
	GameStatistics();

	static GameStatistics* gameStats;
	unsigned int score = 0;
};