#include "Pipe.h"
#include <math.h>
#include <iostream>

Pipe::Pipe() {}
Pipe::Pipe(float translateX,
	float translateY,
	float scaleX,
	float scaleY, 
	float angle, 
	unsigned int id) : Transform(translateX, translateY, scaleX, scaleY, angle), id(id) {
	shrinkSteps = (unsigned int)ceil((scaleY / 15.) * 2);
	enlargeSteps = 0;
	isShrinking = true;
}

void Pipe::shrink() {
	// upper : lower pipes move / don't move also.
	translateY += (translateY >= 0.1f ? 5 : 0);
	scaleY -= 5;
	shrinkSteps--;
	enlargeSteps++;
	isShrinking = (shrinkSteps != 0);
}

void Pipe::enlarge() {
	// upper : lower pipes move / don't move also.
	translateY -= (translateY >= 0.1f ? 5 : 0);
	scaleY += 5;
	enlargeSteps--;
	shrinkSteps++;
	isShrinking = (enlargeSteps == 0);
}