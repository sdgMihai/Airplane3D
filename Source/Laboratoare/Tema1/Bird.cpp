#include <vector>

#include "Bird.h"
#include "Object2D.h"
#include "Tester.h"

void Bird::init() {
	colorBody = glm::vec3(1, 1, 0.6f);
	colorHead = glm::vec3(1, 1, 0.6f);
	colorBeak = glm::vec3(1, 0.4f, 0);
	colorWing = glm::vec3(1, 0.4f, 0);
	colorEye = glm::vec3(1, 0, 0);
	form.setAll(300, 360, 20, 30, 0.f);
	fallSteps = 0;
	riseSteps = 0;
	goingDown = goingUp = 0;
	lastLeftBehindPipe = -1;

	// the Body
	int disp = 1;
	float length = 0.99f;

	int numSegments = 16;
	float rBody = length / 2;
	float cxBody = 1;
	float cyBody = 0.f;
	verticesBody = { VertexFormat(glm::vec3(cxBody, cyBody, 0), colorBody) };

	for (int ii = 0; ii < numSegments; ii++)
	{
		float theta = 2.0f * (float)M_PI * float(ii) / float(numSegments);  // get the current angle

		float x = rBody * cosf(theta);  // calculate the x component
		float y = rBody * sinf(theta);  // calculate the y component

		VertexFormat aux(glm::vec3(x + cxBody, y + cyBody, 0), colorBody);
		verticesBody.push_back(aux);
	}

	for (unsigned short i = 0; i < numSegments - 1; ++i) {
		indicesBody.push_back(0);
		indicesBody.push_back(i + 1u);
		indicesBody.push_back(i + 2u);
	}
	indicesBody.push_back(0);
	indicesBody.push_back(numSegments);
	indicesBody.push_back(1);

	// the Heads
	float rHead = length / 3;
	float cxHead = 0.4f + cxBody;
	float cyHead = 0.4f + rHead;
	verticesHead = { VertexFormat(glm::vec3(cxHead, cyHead, 0), colorHead) };
	for (int ii = 0; ii < numSegments; ii++) 
	{
		float theta = 2.0f * (float)M_PI * float(ii) / float(numSegments);  // get the current angle

		float x = rHead * cosf(theta);  // calculate the x component
		float y = rHead * sinf(theta);  // calculate the y component

		VertexFormat aux(glm::vec3(x + cxHead, y + cyHead, 0), colorHead);
		verticesHead.push_back(aux);
	}

	for (unsigned short i = 0; i < numSegments - 1; ++i) {
		indicesHead.push_back(0);
		indicesHead.push_back(i + 1u);
		indicesHead.push_back(i + 2u);
	}
	indicesHead.push_back(0);
	indicesHead.push_back(numSegments);
	indicesHead.push_back(1);

	// the Beak
	verticesBeak = { VertexFormat(glm::vec3(cxHead + rHead - 0.2f, cyHead - rHead + 0.1f, 0), colorBeak)
		, VertexFormat(glm::vec3(cxHead + rHead + 0.4f, 0.4f + cyHead - rHead, 0), colorBeak)
		, VertexFormat(glm::vec3(cxHead + rHead - 0.2f, 0.6f + cyHead - rHead, 0), colorBeak) };

	indicesBeak = {
		0, 1, 2
	};

	// the Wing
	verticesWing = { VertexFormat(glm::vec3(cxBody - rBody / 2, 0, 0), colorWing)
		, VertexFormat(glm::vec3(cxBody, cyBody - rBody / 2, 0), colorWing)
		, VertexFormat(glm::vec3(cxBody + rBody / 2, 0, 0), colorWing)
	};

	indicesWing = {
		0, 1, 2
	};

	// the eye
	float rEye = length / 7;
	float cxEye = cxHead;
	float cyEye = cyHead;
	verticesEye = { VertexFormat(glm::vec3(cxEye, cyEye, 0), colorEye) };
	for (int ii = 0; ii < numSegments; ii++)
	{
		float theta = 2.0f * (float)M_PI * float(ii) / float(numSegments);  // get the current angle

		float x = rEye * cosf(theta);  // calculate the x component
		float y = rEye * sinf(theta);  // calculate the y component

		VertexFormat aux(glm::vec3(x + cxEye, y + cyEye, 0), colorEye);
		verticesEye.push_back(aux);
	}

	for (unsigned short i = 0; i < numSegments - 1; ++i) {
		indicesEye.push_back(0);
		indicesEye.push_back(i + 1u);
		indicesEye.push_back(i + 2u);
	}
	indicesEye.push_back(0);
	indicesEye.push_back(numSegments);
	indicesEye.push_back(1);
}

void Bird::move(float deltaTime) {
	float dAcc = 3;
	float uAcc = 1;
	
	if (goingDown) {
		if (form.angle > RADIANS(-30)) {
			form.angle -= 8.f * deltaTime;
		}
		form.translateY -= dAcc * fallSteps * fallSteps;
		fallSteps += 0.1f;
	}
	else if (goingUp) {
		if (form.angle < RADIANS(30)) {
			form.angle += 7.f * deltaTime;
		}
		else {
			goingDown = true;
			goingUp = false;
		}
		form.translateY += uAcc * riseSteps * riseSteps;
		riseSteps -= 0.1f;
	}
}

void Bird::startMoving() {
	goingUp = true;
	goingDown = false;
	riseSteps = 4;
	fallSteps = 0;
}

std::pair<float, float> Bird::getT() {
	return std::make_pair(form.translateX, form.translateY);
}

void Bird::setT(const std::pair<float, float>& update) {
	form.translateX = update.first;
	form.translateY = update.second;
}

std::pair<float, float> Bird::getS() {
	return std::make_pair(form.scaleX, form.scaleY);
}

void Bird::setS(const std::pair<float, float>& update) {
	form.scaleX = update.first;
	form.scaleY = update.second;
}

void Bird::setAngle(float angle) {
	form.angle = angle;
}

float Bird::getAngle() {
	return form.angle;
}

int Bird::getFallSteps() {
	return fallSteps;
}

void Bird::setFallSteps(int steps) {
	fallSteps = steps;
}

std::pair<glm::vec2, float> Bird::getBirdCircle() {
	// body X length + head X length + beak length - their common part
	// +0 because is starting in the origin of the axes
	float radius = (((verticesBody[1].position.x - verticesBody.begin()->position.x) + 0.5f + 0.5f) / 2);  // an aproximation
	float cx = radius * form.scaleX
		+ form.translateX;
	float cy = ((verticesBody[2].position.y - verticesBody.begin()->position.y) / 2) * form.scaleY
		+ form.translateY;
	
	return std::make_pair(glm::vec2(cx, cy), radius);
}

//void Bird::init() {
//	color = glm::vec3(0, 0, 1);
//	form.setAll(300, 360, 20, 30, 0.f);
//	fallSteps = 0;
//	riseSteps = 0;
//	goingDown = goingUp = 0;
//	lastLeftBehindPipe = -1;
//
//	// the Body
//	float disp = 1;  // displacement because in center is the head of the bird
//	glm::vec3 leftBottomCorner = glm::vec3(0.001, 0.001, 0);
//	float length = 0.99f;
//
//	glm::vec3 corner = leftBottomCorner;
//	verticesBody =
//	{
//		VertexFormat(corner, color),
//		VertexFormat(corner + glm::vec3(length, 0, 0), color),
//		VertexFormat(corner + glm::vec3(length, length, 0), color),
//		VertexFormat(corner + glm::vec3(0, length, 0), color)
//	};
//
//	indicesBody = { 0, 1, 2, 3, 0, 2 };
//
//	// the Heads
//	int numSegments = 16;
//	float r = length / 2;
//	float cx = 0.5f + disp / 2;
//	float cy = 0.5f;
//	verticesHead = { VertexFormat(glm::vec3(cx, cy, 0), color) };
//	for (int ii = 0; ii < numSegments; ii++)
//	{
//		float theta = 2.0f * (float)M_PI * float(ii) / float(numSegments);  // get the current angle
//
//		float x = r * cosf(theta);  // calculate the x component
//		float y = r * sinf(theta);  // calculate the y component
//
//		VertexFormat aux(glm::vec3(x + cx, y + cy, 0), color);
//		verticesHead.push_back(aux);
//	}
//
//	for (unsigned short i = 0; i < numSegments - 1; ++i) {
//		indicesHead.push_back(0);
//		indicesHead.push_back(i + 1u);
//		indicesHead.push_back(i + 2u);
//	}
//	indicesHead.push_back(0);
//	indicesHead.push_back(numSegments);
//	indicesHead.push_back(1);
//
//	// the Beak
//	verticesBeak = { VertexFormat(glm::vec3(0 + length + disp / 2, 0, 0), color)
//		, VertexFormat(glm::vec3(0.5f + length + disp / 2, 0.5f, 0), color)
//		, VertexFormat(glm::vec3(0 + length + disp / 2, 1, 0), color) };
//
//	indicesBeak = {
//		0, 1, 2
//	};
//}