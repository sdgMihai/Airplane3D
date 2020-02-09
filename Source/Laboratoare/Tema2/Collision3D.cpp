#include "Collision3D.h"

#include <iostream>
#include <Windows.h>
#include "Tester.h"
#include "GameStatistics3D.h"

Collision3D* Collision3D::collision = nullptr;

Collision3D* Collision3D::getCollision3D() {
	if (collision != nullptr) {
		return collision;
	}
	else {
		std::cout << "error getting collision class";
		exit(-2);
	}
}

Collision3D* Collision3D::getCollision3D(Plane& bird, Obstacles& blocks, Obstacles& fuelRegs) {
	if (collision == nullptr) {
		collision = new Collision3D(bird, blocks, fuelRegs);
	}
	return collision;
}

Collision3D::Collision3D(Plane& bird, Obstacles& blocks, Obstacles& fuelRegs)
	: plane(bird), blocks(blocks), fuelRegs(fuelRegs) {}

Collision3D::~Collision3D() {
	if (collision != nullptr) {
		delete collision;
		collision = nullptr;
	}
}

bool Collision3D::testAndApplyCollision() {
	std::vector<Form3D> parts = { plane.formBody, plane.formW1, plane.formW2 };
	for (const auto& planePart : parts) {
		for (auto it = blocks.forms.begin(); it != blocks.forms.end();) {
			assert(it != blocks.forms.end());
			auto obstacle = *it;
			
			if (minX(planePart) <= maxX(obstacle) && maxX(planePart) >= minX(obstacle)
				&& minY(planePart) <= maxY(obstacle) && maxY(planePart) >= minY(obstacle)
				&& minZ(planePart) <= maxZ(obstacle) && maxZ(planePart) >= minZ(obstacle)) {
				
				assert(it != blocks.forms.end());
				blocks.forms.erase(it);
				it = blocks.forms.begin();

				// collision with obstacle -> lose one life
				GameStatistics3D::getGameStatistics()->deleteHeart();
				return true;
			}
			
			else {
				assert(it != blocks.forms.end());
				it++;
			}
		}

		for (auto it = fuelRegs.forms.begin(); it != fuelRegs.forms.end();) {
			assert(it != fuelRegs.forms.end());
			auto obstacle = *it;

			if (minX(planePart) <= maxX(obstacle) && maxX(planePart) >= minX(obstacle)
				&& minY(planePart) <= maxY(obstacle) && maxY(planePart) >= minY(obstacle)
				&& minZ(planePart) <= maxZ(obstacle) && maxZ(planePart) >= minZ(obstacle)) {

				assert(it != fuelRegs.forms.end());
				fuelRegs.forms.erase(it);
				it = fuelRegs.forms.begin();
				GameStatistics3D::getGameStatistics()->gotFuelBonus();
				return true;
			}

			else {
				assert(it != fuelRegs.forms.end());
				it++;
			}
		}
	}
	/*Tester::getTester()->printForm(obstacle);
	Tester::getTester()->delimiterMsg();*/
	return false;
}

int Collision3D::updateScore() {
	//if (bird.lastLeftBehindPipe == -1) {
	//	auto pipe = blocks.forms.begin();

	//	if (pipe->translateX + pipe->scaleX < bird.form.translateX) {
	//		bird.lastLeftBehindPipe = pipe->id;
	//		return 1;
	//	}
	//} else {
	//	for (auto const& pipe : blocks.forms) {
	//		if (pipe.id == (bird.lastLeftBehindPipe + 2) % 10) {
	//			// test if the next pipe after last left behind
	//			// is also passed
	//			if (pipe.translateX + pipe.scaleX < bird.form.translateX) {
	//				bird.lastLeftBehindPipe = pipe.id;  // vezi care e faca
	//				return 1;
	//			}
	//		}
	//	}
	//}
	//
	return 0;
}

float Collision3D::minX(const Form3D& form) {
	return form.translateX - 1 * form.scaleX; //  Plane::l / 2;
}

float Collision3D::maxX(const Form3D& form) {
	return form.translateX + 1 * form.scaleX; //  +Plane::l / 2;
}

float Collision3D::minY(const Form3D& form) {
	return form.translateY - 1 * form.scaleY; // - Plane::l / 2;
}

float Collision3D::maxY(const Form3D& form) {
	return form.translateY + 1 * form.scaleY; // + Plane::l / 2;
}

float Collision3D::minZ(const Form3D& form) {
	return form.translateZ - 1 * form.scaleZ; // - Plane::l / 2;
}

float Collision3D::maxZ(const Form3D& form) {
	return form.translateZ + 1 * form.scaleZ; // + Plane::l / 2;
}
