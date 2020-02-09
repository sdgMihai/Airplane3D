#include <vector>
#include <iostream>
#include <cmath>

#include "Plane.h"
#include "Tester.h"

void Plane::init() {
	verticesBody = 
	{
		VertexFormat(glm::vec3(-1, -1,  1), glm::vec3(0, 1, 1), glm::vec3(0.2, 0.8, 0.2)),
		VertexFormat(glm::vec3(1, -1,  1), glm::vec3(1, 0, 1), glm::vec3(0.9, 0.4, 0.2)),
		VertexFormat(glm::vec3(-1,  1,  1), glm::vec3(1, 0, 0), glm::vec3(0.7, 0.7, 0.1)),
		VertexFormat(glm::vec3(1,  1,  1), glm::vec3(0, 1, 0), glm::vec3(0.7, 0.3, 0.7)),
		VertexFormat(glm::vec3(-1, -1, -1), glm::vec3(1, 1, 1), glm::vec3(0.3, 0.5, 0.4)),
		VertexFormat(glm::vec3(1, -1, -1), glm::vec3(0, 1, 1), glm::vec3(0.5, 0.2, 0.9)),
		VertexFormat(glm::vec3(-1,  1, -1), glm::vec3(1, 1, 0), glm::vec3(0.7, 0.0, 0.7)),
		VertexFormat(glm::vec3(1,  1, -1), glm::vec3(0, 0, 1), glm::vec3(0.1, 0.5, 0.8)),
	};

	indicesBody =
	{
		0, 1, 2,		1, 3, 2,
		2, 3, 7,		2, 7, 6,
		1, 7, 3,		1, 5, 7,
		6, 7, 4,		7, 5, 4,
		0, 4, 1,		1, 4, 5,
		2, 6, 4,		0, 2, 4,
	};

	formBody.setTranslate(0, 0, 0);
	formBody.setScale(0.18f, 0.1f, 0.1f);
	formBody.setAngle(0, 0, 0);

	formB1.setScale(0.02f, 0.01f, 0.2f);

	formB1.setTranslate(formBody.translateX + (formBody.scaleX ) * l / 2
		, formBody.translateY - 1.3f
		, formBody.translateZ);
	formB1.angle = 0;
	formB1.setAngle(0, 0, 0);

	formW1.setTranslate(formBody.translateX
		, formBody.translateY
		, formBody.translateZ );
	formW1.setScale(0.05f, 0.01f, 0.2f);
	formW1.setAngle(0, 0, 0);

	/*formW2.setTranslate(formBody.translateX
		, formBody.translateY
		, formBody.translateZ - l / 2 * formBody.scaleZ);
	formW2.setScale(0.05f, 0.01f, 0.2f);
	formW2.setAngle(0, 0, 0);*/
}

void Plane::updateForm() {

	modelMatrixBody = glm::mat4(1);
	modelMatrixBody = glm::translate(modelMatrixBody, formBody.getTranslate());
	modelMatrixBody = glm::scale(modelMatrixBody, formBody.getScale());

	modelMatrixB1 = glm::mat4(1);
	modelMatrixB1 = glm::translate(modelMatrixB1, formB1.getTranslate());
	modelMatrixB1 = glm::rotate(modelMatrixB1, formB1.angleZY, glm::vec3(1, 0, 0));
	modelMatrixB1 = glm::scale(modelMatrixB1, formB1.getScale());

	modelMatrixW1 = glm::mat4(1);
	modelMatrixW1 = glm::translate(modelMatrixW1, formW1.getTranslate());
	// modelMatrixW1 = glm::rotate(modelMatrixW1, RADIANS(60.0f), glm::vec3(1, 1, 0));
	modelMatrixW1 = glm::scale(modelMatrixW1, formW1.getScale());
}

void Plane::move(float deltaTime) {
	// there could be improvements
	float uAcc = 200;
	formB1.angleZY += deltaTime * 4;

	if (formBody.translateY - yTarget > 0.001) {
		formBody.translateY = (formBody.translateY + yTarget) / 2;
		formB1.setTranslate(formBody.translateX + (formBody.scaleX + formB1.scaleX) * l / 2
			, formBody.translateY
			, formBody.translateZ);
		formW1.setTranslate(formBody.translateX
			, formBody.translateY
			, formBody.translateZ);
	} else if (formBody.translateY - yTarget < -0.001) {
		formBody.translateY = (formBody.translateY + yTarget) / 2;
		formB1.setTranslate(formBody.translateX + (formBody.scaleX + formB1.scaleX) * l / 2
			, formBody.translateY
			, formBody.translateZ);
		formW1.setTranslate(formBody.translateX
			, formBody.translateY
			, formBody.translateZ);
	}
}

void Plane::startMoving(float deltaTime, float mouseY) {
	// 575 is the y pos of Ox axis on the game view in mouse coord(given the window has 720 px on Y)
	mouseY = 575 - mouseY;
	this->yTarget = mouseY / 720.f * 6.f;

	formBody.angleXY = atan(yTarget - formBody.translateY);
	formB1.angleXY = formBody.angleXY;
	formW1.angleXY = formBody.angleXY;
}

std::pair<glm::vec2, float> Plane::getPlaneCircle() {
	// body X length + head X length + beak length - their common part
	// +0 because is starting in the origin of the axes
	//float radius = (((verticesBody[1].position.x - verticesBody.begin()->position.x) + 0.5f + 0.5f) / 2);  // an aproximation
	//float cx = radius * form.scaleX
	//	+ form.translateX;
	//float cy = ((verticesBody[2].position.y - verticesBody.begin()->position.y) / 2) * form.scaleY
	//	+ form.translateY;

	//return std::make_pair(glm::vec2(cx, cy), radius);
	return std::make_pair(glm::vec2(-1, -1), -1);
}