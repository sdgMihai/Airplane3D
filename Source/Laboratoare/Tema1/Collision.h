#pragma once

#include "Bird.h"
#include "Blocks.h"

class Collision {
public:
	Bird& bird;
	Blocks& blocks;
	static Collision* getCollision();
	static Collision* getCollision(Bird& ,Blocks& );
	bool testCollision()const;
	int updateScore();

private:
	Collision(Bird&, Blocks&);
	Collision();
	~Collision();
	static Collision* collision;
};