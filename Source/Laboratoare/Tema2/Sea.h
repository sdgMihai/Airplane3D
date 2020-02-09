#pragma once

#include <vector>

#include <Component/SimpleScene.h>
#include <Core/Engine.h>

#include "Form3D.h"


class Sea {
public:
	void init(float translateX);

	void updateForm();

	void move(float);

	void startMoving();

	std::vector<unsigned short> indices;
	std::vector<VertexFormat> vertices;

	Form3D form;

	glm::mat4 modelMatrix;

	glm::vec3 colorBody;
};