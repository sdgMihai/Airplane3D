#pragma once
#include <fstream>
#include <iostream>
#include <glm/gtx/string_cast.hpp>

#include "Plane.h"
#include "Form3D.h"
#include "Laboratoare/Laborator5/LabCamera.h"

class Tester {
public:
	static Tester* getTester();

	/* used imd after plane.move()
	*/
	void verifyApproach(Plane&);

	/* used in Tema2.cpp to test size of window, etc
	*/
	void translatePlane(Plane&);

	/* used imd after plane.startMoving()
	*/
	void startMoving(Plane&);

	void printMat4(const glm::mat4& mat);

	void printVec3(const glm::vec3& vec);

	void printForm(const Form3D& form);

	void printCamera(const Laborator::Camera& camera);
	
	void delimiterMsg();

	~Tester();
private:
	Tester();

	std::ofstream out;

	static Tester* tester;
};