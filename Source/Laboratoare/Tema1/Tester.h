#pragma once

#include <fstream>
#include <string>
#include <cstddef>
#include "Blocks.h"
#include "Bird.h"
#include "Transform.h"
#include "Collision.h"
#include "Pipe.h"

class Tester {
public:
	std::ofstream out;

	static Tester* getTester();

	void printBlocks(const Blocks*);

	void printBird(const Bird*);

	void printTransform(const Transform*);

	void printCollision(const Collision*);

	void printPipe(const Pipe*);

	~Tester();
private:
	Tester();

	static Tester* tester;
};