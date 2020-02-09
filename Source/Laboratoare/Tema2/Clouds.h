#pragma once

#include <list>

#include "Form3D.h"

/* Clouds aren't dangerous for the plane. But i used them as obstacles for dev speed.
*/
class Clouds {
public:
	/* Initializes the object fields and it adds one cloud to forms.
	*/
	void init(glm::vec3&, float yLimit = -2.7f);

	/* Creates and adds one cloud to form. Currently a clouds is made out of 
	4 squares.
	*/
	void createAndAddCloud(float translateX);

	/* Pops the first created cloud from forms.
	*/
	void destroyCloud();

	void move(float deltaTimeSeconds);

	/* Updates the modelMatrix for the object with params modified in move stage,
	as described by function move.
	*/
	void updateForm(const Form3D& formBody);

	std::list<Form3D> forms;
	float yLowerLimit;
	glm::mat4 modelMatrix;
	/* Keeps track of time since the last cloud created.
	*/
	float elapsedTime;
	glm::vec3 color;
};