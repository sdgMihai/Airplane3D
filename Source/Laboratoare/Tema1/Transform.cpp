#include "Transform.h"

Transform::Transform() {}

Transform::Transform(const Transform& x) {
	this->scaleX = x.scaleX;
	this->scaleY = x.scaleY;
	this->translateX = x.translateX;
	this->translateY = x.translateY;
	this->angle = x.angle;
}

Transform::Transform(float translateX, float translateY, float scaleX, float scaleY, float angle)
	: translateX(translateX), translateY(translateY), scaleX(scaleX), scaleY(scaleY), angle(angle) {}

void Transform::setAll(float translateX, float translateY, float scaleX, float scaleY, float angle) {
	this->translateX = translateX;
	this->translateY = translateY;
	this->scaleX = scaleX;
	this->scaleY = scaleY;
	this->angle = angle;
}