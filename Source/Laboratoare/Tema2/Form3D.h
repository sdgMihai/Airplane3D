#pragma once

#include "Laboratoare/Tema1/Transform.h"
#include <Core/GPU/Mesh.h>

/*
	Form3D saves the position, scale and rotation data about an object
	in 3D space.
	It keeps the angle field for inheritance purpose only, and we strongly
	discourage it's use, as it has been replaced with angleXY, angleZY
	and angleXZ.
*/
class Form3D : public Transform {
public:
	Form3D();
	Form3D(float translateX, float translateY, float translateZ
		, float scaleX, float scaleY, float scaleZ
		, float angleXY, float angleZY, float angleXZ);
	Form3D(const Form3D& cp);

	glm::vec3 getTranslate() const;
	void setTranslate(float translateX, float translateY, float translateZ);
	void addTranslate(float translateX, float translateY, float translateZ);

	glm::vec3 getScale() const;
	void setScale(float scaleX, float scaleY, float scaleZ);

	/* Set the angles for rotating the object.
	@param1 - rotating angle round the Z ax.
	@param2 - rotating angle round the X ax.
	@param3 - rotating angle round the Y ax.
	*/
	void setAngle(float, float, float);

	float translateZ;
	float scaleZ;
	float angleXY;
	float angleZY;
	float angleXZ;
};
