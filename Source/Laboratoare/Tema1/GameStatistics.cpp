#include "GameStatistics.h"
#include "Collision.h"

GameStatistics* GameStatistics::gameStats = nullptr;

GameStatistics::GameStatistics() {}

void GameStatistics::printStartingMessage() {
	std::cout << "Starting Game!!\n Press space to start and fly!\n";
}

GameStatistics::~GameStatistics() {
	if (gameStats != nullptr) {
		delete gameStats;
		gameStats = nullptr;
	}
}

GameStatistics* GameStatistics::getGameStatistics() {
	if (gameStats == nullptr) {
		gameStats = new GameStatistics();
	}

	return gameStats;
}

void GameStatistics::update() {
	int t = Collision::getCollision()->updateScore();
	if (t == 1) {
		score++;
		std::cout << "Your score is " << score << '\n';
	}
}

void GameStatistics::printEndingMessage() {
	std::cout << "End of game!";
	std::cout << "Your score is " << score << '\n';
}

unsigned int GameStatistics::getScore() {
	return score;
}