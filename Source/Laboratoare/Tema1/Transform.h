#pragma once

class Transform {
public:
	float translateX;
	float translateY;
	float scaleX;
	float scaleY;
	float angle;

	Transform();

	Transform(float translateX, float translateY, float scaleX, float scaleY, float angle);

	Transform(const Transform& x);
	
	void setAll(float translateX, float translateY, float scaleX, float scaleY, float angle);
};