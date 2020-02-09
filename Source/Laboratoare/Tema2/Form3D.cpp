#include "Form3D.h"
#include <Core/GPU/Mesh.h>

Form3D::Form3D() {}

Form3D::Form3D(float translateX, float translateY, float translateZ
	, float scaleX, float scaleY, float scaleZ
	, float angleXY, float angleZY, float angleXZ) : Transform(translateX, translateY, scaleX, scaleY, angleXY)
		, translateZ(translateZ)
		, scaleZ(scaleZ)
		, angleXY(angleXY)
		, angleZY(angleZY)
		, angleXZ(angleXZ) {}

Form3D::Form3D(const Form3D& cp) : Transform(cp)
	, translateZ(cp.translateZ)
	, scaleZ(cp.scaleZ)
	, angleXY(cp.angleXY)
	, angleZY(cp.angleZY)
	, angleXZ(cp.angleXZ) {}

glm::vec3 Form3D::getTranslate() const {
	return glm::vec3(translateX, translateY, translateZ);
}

void Form3D::setTranslate(float translateX, float translateY, float translateZ) {
	this->translateX = translateX;
	this->translateY = translateY;
	this->translateZ = translateZ;
}

void Form3D::addTranslate(float translateX, float translateY, float translateZ) {
	this->translateX += translateX;
	this->translateY += translateY;
	this->translateZ += translateZ;
}


glm::vec3 Form3D::getScale() const {
	return glm::vec3(scaleX, scaleY, scaleZ);
}

void Form3D::setScale(float scaleX, float scaleY, float scaleZ) {
	this->scaleX = scaleX;
	this->scaleY = scaleY;
	this->scaleZ = scaleZ;
}

void Form3D::setAngle(float angleXY, float angleZY, float angleXZ) {
	this->angleXY = angleXY;
	this->angleZY = angleZY;
	this->angleXZ = angleXZ;
}