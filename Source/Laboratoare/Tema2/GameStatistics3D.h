#pragma once
#include <vector>
#include "Form3D.h"

class GameStatistics3D {
public:
	void init();

	/* Updates all the model matrixes of game stats.
	As of now, there are the fuel bar and lives hearts to be updated.
	*/
	void updateModelMatrix();

	void printStartingMessage();

	static GameStatistics3D* getGameStatistics();

	/* Update score and quantity of fuel at player's disposal at given frame.
	*/
	void update(float);

	void printEndingMessage();

	unsigned int getScore();

	/* Pop back one life and dec lives.
	*/
	void deleteHeart();

	/* If the plane is within eye sight -> game is ready to play.
	*/
	bool isGameReady(float, bool);

	/* If there's no fuel or no of lives = 0,
	then game is over
	*/
	bool isGameOver();

	void gotFuelBonus();

	~GameStatistics3D();

	/* Fuel.
	*/
	glm::mat4 modelMatrixBack;
	glm::vec3 colorBack;
	glm::mat4 modelMatrixFront;
	glm::vec3 colorFront;

	/* This two fields store the fuel form information.
	@quantity of fuel = front.scaleX
	*/
	Form3D back;
	Form3D front;


	/* Here are the lives' information(no and form).
	The heart form3D vector is allocated in constructor.
	*/
	std::vector<Form3D> heartForms;
	/* Allocated in constructor, and updated in function update.
	*/
	std::vector<glm::mat4> modelMatrixHeart;
private:
	/* Set default score and no of lives in fields score, respective lives.
	*/
	GameStatistics3D();

	static GameStatistics3D* gameStats;
	/* Speed increases with score. As a result changes to 'score' field must take this into consideration.
	*/
	unsigned int score;

};