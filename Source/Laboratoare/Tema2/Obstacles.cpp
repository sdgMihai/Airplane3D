#include "Obstacles.h"
#include <cstdlib>
#include <time.h>       /* time */

#include "GameStatistics3D.h"
#include "Laboratoare/Laborator4/Transform3D.h"	
#include "Tester.h"

void Obstacles::init(glm::vec3& color, int nrOfObstacles, float yLimit) {
	this->color = color;
	this->nrOfObstacles = nrOfObstacles;
	yLowerLimit = yLimit;
	createAndAddObstacle(2.5f);
	elapsedTime = 0;
}

void Obstacles::createAndAddObstacle(float translateX) {
	forms.push_back(Form3D(translateX, yLowerLimit, 0, 0.05f, 0.05f, 0.05f, 0, 0, 0));
}

void Obstacles::destroyObstacle() {
	forms.pop_front();
}

void Obstacles::move(float deltaTimeSeconds) {
	int speed = 1.f;
	for (auto it = forms.begin(); it != forms.end(); it++) {
		// move object
		it->translateX -= deltaTimeSeconds * speed;
		it->translateY += sin(Engine::GetElapsedTime()) / 100;

	}
	
	if (forms.size() > nrOfObstacles) {
		for (auto it = forms.begin(); it != forms.end();) {
			// if obj out of image, destroy it and replace with one at the other side
			if (it->translateX < -3) {
				destroyObstacle();
				it = forms.begin();
				createAndAddObstacle(2.5f);
			} else {
				it++;
			}
		}

	} else {
		elapsedTime += deltaTimeSeconds;
		if (elapsedTime > 0.25f) {
			createAndAddObstacle(yLowerLimit);
			/*Tester::getTester()->printForm();*/
			elapsedTime = 0;
		}
	}
}

void Obstacles::updateForm(const Form3D& formBody) {
	//modelMatrix = glm::mat4(1);
	//modelMatrix *= Transform3D::Translate(0.0f, -formBody.translateY, formBody.translateZ);  // (0.0f, 0.5f, -1.5f
	//modelMatrix *= Transform3D::RotateOZ(formBody.angleXZ);

	//modelMatrix *= Transform3D::Translate(formBody.translateX, -0.0f, 0.0f);
	//modelMatrix *= Transform3D::RotateOZ(formBody.angleXZ);

	//modelMatrix *= Transform3D::RotateOX(formBody.angleZY);
	//modelMatrix = glm::scale(modelMatrix, formBody.getScale());
	// Tester::getTester()->printMat4(modelMatrix);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(formBody.translateX, formBody.translateY, formBody.translateZ);
	modelMatrix *= Transform3D::Scale(formBody.scaleX, formBody.scaleY, formBody.scaleZ);
}