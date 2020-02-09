#pragma once
#include "Transform.h"
class Pipe : public Transform {
public:
	unsigned int id;
	bool isShrinking;
	unsigned int shrinkSteps;
	unsigned int enlargeSteps;
	Pipe();

	Pipe(float translateX,
		float translateY,
		float scaleX,
		float scaleY,
		float angle, 
		unsigned int);

	// the pipe shrinks and auto repositions itself
	void shrink();
	// the pipe enlarges and auto repositions itself
	void enlarge();
};