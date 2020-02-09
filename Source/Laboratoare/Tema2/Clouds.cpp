#include "Clouds.h"
#include <cstdlib>
#include <time.h>       /* time */

#include "GameStatistics3D.h"
#include "Laboratoare/Laborator4/Transform3D.h"	
#include "Tester.h"

unsigned short lfsr = 0xACE1u;
unsigned bit;

unsigned myRand()
{
	bit = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5)) & 1;
	return lfsr = (lfsr >> 1) | (bit << 15);
}

void Clouds::init(glm::vec3& color, float yLimit) {
	this->color = color;
	yLowerLimit = yLimit;
	createAndAddCloud(2.5f);
	elapsedTime = 0;
}

void Clouds::createAndAddCloud(float translateX) {
	srand(time(NULL));
	float height = yLowerLimit + (float)(myRand() % (1)) * 7 + (float)(myRand() % (10)) / 10 + (float)(myRand() % (100)) / 100;
	forms.push_back(Form3D(translateX, -1, height, 0.05f, 0.05f, 0.05f, 0, 0, 0));
	forms.push_back(Form3D(translateX - 0.08f, -1, height, 0.03f, 0.02f, 0.03f, 0, 0, 0));
	forms.push_back(Form3D(translateX + 0.08f, -1, height, 0.03f, 0.02f, 0.03f, 0, 0, 0));
	forms.push_back(Form3D(translateX - 0.08f, -1.1f, height, 0.03f, 0.02f, 0.03f, 0, 0, 0));
}

void Clouds::destroyCloud() {
	forms.pop_back();
	forms.pop_back();
	forms.pop_back();
	forms.pop_back();
}

void Clouds::move(float deltaTimeSeconds) {
	int speed = 1.f;
	for (auto& elem : forms) {
		elem.angleXZ += deltaTimeSeconds * speed;
		elem.angleXY += deltaTimeSeconds * speed;
		elem.angleZY += deltaTimeSeconds * speed;
		elem.translateY += sin(Engine::GetElapsedTime()) / 100;
		elem.translateZ += sin(Engine::GetElapsedTime()) / 50;
	}
	for (auto it = forms.begin(); it != forms.end();) {
		if (it->translateX < -3) {
			destroyCloud();
			it = forms.begin();
			createAndAddCloud(1);
		} else {
			it++;
		}
	}
	elapsedTime += deltaTimeSeconds;
	if (elapsedTime > 0.1f && forms.size() < 24) {
		createAndAddCloud(2.4f);
		elapsedTime = 0;
	}
	
}

void Clouds::updateForm(const Form3D& formBody) {
	// translateX var
	/*modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, formBody.getTranslate());
	modelMatrix = glm::scale(modelMatrix, formBody.getScale());*/

	// todo make the translate according to each form.translate values
	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(0.0f, -formBody.translateY, formBody.translateZ);  // (0.0f, 0.5f, -1.5f
	modelMatrix *= Transform3D::RotateOZ(formBody.angleXZ);

	modelMatrix *= Transform3D::Translate(formBody.translateX, -0.0f, 0.0f);
	modelMatrix *= Transform3D::RotateOZ(formBody.angleXZ);

	modelMatrix *= Transform3D::RotateOX(formBody.angleZY);
	modelMatrix = glm::scale(modelMatrix, formBody.getScale());
	// Tester::getTester()->printMat4(modelMatrix);
}