#pragma once

#include <Component/SimpleScene.h>
#include <string>
#include <Core/Engine.h>
#include <vector>
#include <utility>
#include "Transform.h"

class Bird {
 public:

	void init();

	void move(float deltaTime);

	void startMoving();

	std::pair<float, float> getT();

	std::pair<float, float> getS();

	void setT(const std::pair<float, float>&);

	void setS(const std::pair<float, float>&);

	void setAngle(float angle);

	float getAngle();

	int getFallSteps();

	void setFallSteps(int);

	std::pair<glm::vec2, float> getBirdCircle();

	 std::vector<unsigned short> indicesBody;
	 std::vector<unsigned short> indicesHead;
	 std::vector<unsigned short> indicesBeak;
	 std::vector<unsigned short> indicesWing;
	 std::vector<unsigned short> indicesEye;

	 std::vector<VertexFormat> verticesBody;
	 std::vector<VertexFormat> verticesHead;
	 std::vector<VertexFormat> verticesBeak;
	 std::vector<VertexFormat> verticesWing;
	 std::vector<VertexFormat> verticesEye;

	 glm::vec3 colorBody;
	 glm::vec3 colorHead;
	 glm::vec3 colorBeak;
	 glm::vec3 colorWing;
	 glm::vec3 colorEye;
	 Transform form;

	 float fallSteps;
	 float riseSteps;

	 bool goingUp;
	 bool goingDown;

	 int lastLeftBehindPipe;
};