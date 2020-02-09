#pragma once

#include <Component/SimpleScene.h>
#include <Core/Engine.h>
#include <string>
#include <vector>
#include <utility>

#include "Form3D.h"

class Plane {
public:

	void init();

	/*
	Updates form for all parts of the plane.
	*/
	void updateForm();

	/* The plane moves, following the y position of the mouse.
	*/
	void move(float);

	/* Updates the position of the mouse, starting new movements chain
	at each mouse move.
	*/
	void startMoving(float deltaTime, float y);

	/* useless at the moment
	*/
	std::pair<glm::vec2, float> getPlaneCircle();

	std::vector<unsigned short> indicesBody;

	std::vector<VertexFormat> verticesBody;
	

	glm::vec3 colorBody;

	Form3D formBody;
	Form3D formB1;
	Form3D formW1, formW2;

	glm::mat4 modelMatrixBody;
	glm::mat4 modelMatrixB1 = glm::mat4(1);
	glm::mat4 modelMatrixB2 = glm::mat4(1);
	glm::mat4 modelMatrixW1 = glm::mat4(1);
	glm::mat4 modelMatrixW2 = glm::mat4(1);

	/* the absolute
	position of the mouse using the 3D coord's y , not the window's y.
	*/
	float yTarget;

	// TODO must be used or removed.
	int lastLeftBehindPipe;

	// Create a simple cube with l = 2
	// change l accordingly to the dist between the vertices of Body
	float l = 2;
};