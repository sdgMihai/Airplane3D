#include "GameStatistics3D.h"
#include "Collision3D.h"

//TODO partial modified

GameStatistics3D* GameStatistics3D::gameStats = nullptr;

GameStatistics3D::GameStatistics3D() {
	score = 0;
	int lives = 3;
	modelMatrixHeart.resize(lives);
	heartForms.resize(lives);
}

void GameStatistics3D::init() {
	// fuel
	Form3D commonFuel;
	// just X and Y matters
	commonFuel.setTranslate(2.8f, 4, 0);
	commonFuel.setScale(0.5f, 0.3f, 0.3f);
	back.setTranslate(commonFuel.translateX, commonFuel.translateY, 0);
	back.setScale(commonFuel.scaleX, commonFuel.scaleY, 0.3f);
	back.angleXZ = 0;
	colorBack = glm::vec3(0, 0, 0);
	front.setTranslate(commonFuel.translateX, commonFuel.translateY, 0.2f);
	front.setScale(commonFuel.scaleX - 0.02f, commonFuel.scaleY - 0.02f, 0.1f);
	front.angleXZ = 0;
	colorFront = glm::vec3(1, 0, 0);
	// lives
	Form3D formLife;
	formLife.setTranslate(-2.4f, 4.5f, 0);
	formLife.setScale(0.1f, 0.1f, 0.1f);
	float space = 0.3f;

	for (int i = 0; i < heartForms.size(); ++i) {
		heartForms[i].setTranslate(formLife.translateX + space * i, formLife.translateY, formLife.translateZ);
		heartForms[i].setScale(formLife.scaleX, formLife.scaleY, formLife.scaleZ);
	}
}

void GameStatistics3D::updateModelMatrix() {
	// fuel
	// the back panel
	modelMatrixBack = glm::mat4(1);
	modelMatrixBack = glm::translate(modelMatrixBack, back.getTranslate());
	modelMatrixBack = glm::scale(modelMatrixBack, back.getScale());
	modelMatrixBack = glm::rotate(modelMatrixBack, RADIANS(back.angleXZ), glm::vec3(0, 1, 0));
	// the front panel
	modelMatrixFront = glm::mat4(1);
	modelMatrixFront = glm::translate(modelMatrixFront, front.getTranslate());
	modelMatrixFront = glm::scale(modelMatrixFront, front.getScale());
	modelMatrixFront = glm::rotate(modelMatrixFront, RADIANS(front.angleXZ), glm::vec3(0, 1, 0));
	// DONE : the hearts
	for (int i = 0; i < heartForms.size(); ++i) {
		modelMatrixHeart[i] = glm::mat4(1);
		modelMatrixHeart[i] = glm::translate(modelMatrixHeart[i], heartForms[i].getTranslate());
		modelMatrixHeart[i] = glm::scale(modelMatrixHeart[i], heartForms[i].getScale());
		modelMatrixHeart[i] = glm::rotate(modelMatrixHeart[i], RADIANS(heartForms[i].angleXZ), glm::vec3(0, 1, 0));
	}
}

void GameStatistics3D::printStartingMessage() {
}

GameStatistics3D::~GameStatistics3D() {
	if (gameStats != nullptr) {
		delete gameStats;
		gameStats = nullptr;
	}
}

GameStatistics3D* GameStatistics3D::getGameStatistics() {
	if (gameStats == nullptr) {
		gameStats = new GameStatistics3D();
	}

	return gameStats;
}

void GameStatistics3D::update(float deltaTimeSeconds) {
	// t == 1 if there was collision avoided
	//int t = Collision3D::getCollision()->updateScore();
	int t = 0;  // TODO remove the comments and this line ->it's for testing purpose only
	if (t == 1) {
		score++;
		//std::cout << "Your score is " << score << '\n';
	}
	// because speed d.p. with score
	front.scaleX = (front.scaleX > 0.01f ? 
		front.scaleX - (score + 1) * deltaTimeSeconds / 100
		: 0); //  Engine::GetElapsedTime();
}

void GameStatistics3D::printEndingMessage() {
	/*std::cout << "End of game!";
	std::cout << "Your score is " << score << '\n';*/
}

unsigned int GameStatistics3D::getScore() {
	return score;
}

bool GameStatistics3D::isGameReady(float translateY, bool started) {
	// if plane within eye sight or game already started
	return started || (translateY < 4.79 && translateY > 0);
}

void GameStatistics3D::deleteHeart() {
	heartForms.pop_back();
	modelMatrixHeart.pop_back();
}

bool GameStatistics3D::isGameOver() {
	if (front.scaleX <= 0.01f || heartForms.empty()) {
		return true;
	}
	return false;
}

void GameStatistics3D::gotFuelBonus() {
	front.scaleX += (front.scaleX + 0.05f < back.scaleX ? 0.05f : 0);
}