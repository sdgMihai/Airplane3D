#include "Tester.h"

Tester* Tester::tester = nullptr;

Tester::Tester() {
	out.open(".\\Source\\Laboratoare\\Tema2\\log1.out");
}

Tester::~Tester() {
	if (tester != nullptr) {
		delete tester;
		tester = nullptr;
	}
}

void Tester::verifyApproach(Plane& plane) {
	out << "ytarget: " << plane.yTarget << "translateY " << plane.formBody.translateY << '\n';
	printMat4(plane.modelMatrixBody);
}

void Tester::translatePlane(Plane& plane) {
	float translateY;
	std::cin >> translateY;
	plane.formBody.translateY = translateY;
	out << "plane.formBody.translateY " << plane.formBody.translateY << '\n';
}

void Tester::startMoving(Plane& plane) {
	out << "start Moving " << plane.yTarget << '\n';
}

void Tester::printMat4(const glm::mat4& mat) {
	out << glm::to_string(mat) << '\n';
}

void Tester::printVec3(const glm::vec3& vec) {
	out << glm::to_string(vec) << '\n';
}

Tester* Tester::getTester() {
	if (tester == nullptr) {
		tester = new Tester();
	}

	return tester;
}

void Tester::printForm(const Form3D& form) {
	out << form.translateX << ' ' << form.translateY << ' ' << form.translateZ << ' ';
	out << form.scaleX << ' ' << form.scaleY << ' ' << form.scaleZ << ' ';
	out << form.angleXY << ' ' << form.angleXZ << ' ' << form.angleZY << '\n';
}

void Tester::printCamera(const Laborator::Camera& camera) {
	out << "printing camera" << '\n';
	out << "position : ";
	printVec3(camera.position);
	out << "up : ";
	printVec3(camera.up);
}

void Tester::delimiterMsg() {
	out << "Delimiter mesage\n";
	out << '\n';
}
