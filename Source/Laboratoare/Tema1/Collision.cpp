#include "Collision.h"
#include <iostream>

Collision* Collision::collision = nullptr;

Collision* Collision::getCollision() {
	if (collision != nullptr) {
		return collision;
	}
	else {
		std::cout << "error getting collision class";
		exit(-2);
	}
}

Collision* Collision::getCollision(Bird& bird, Blocks& blocks) {
	if (collision == nullptr) {
		collision = new Collision(bird, blocks);
	}
	return collision;
}

Collision::Collision(Bird& bird, Blocks& blocks) : bird(bird), blocks(blocks){}

Collision::~Collision() {
	if (collision != nullptr) {
		delete collision;
		collision = nullptr;
	}
}

bool Collision::testCollision() const{
	// test bird is in the window
	if (bird.form.translateY > blocks.resolution.y
		|| bird.form.translateY < 0) {
		return true;
	}
	// test bird collides with pipes
	// Get center point circle first
	glm::vec2 center = bird.getBirdCircle().first;
	float radius = bird.getBirdCircle().second;

	for (auto const& pipe : blocks.forms) {
		glm::vec2 aabb_half_extents(pipe.scaleX / 2, pipe.scaleY / 2);
		glm::vec2 aabb_center(pipe.translateX + aabb_half_extents.x
			, pipe.translateY + aabb_half_extents.y
		);
		// Get difference vector between both centers
		glm::vec2 difference = center - aabb_center;
		glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
		// add clamped value to aabb_center and we get the value of box closest to circle
		glm::vec2 closest = aabb_center + clamped;
		// retrieve vector between center circle and closest point aabb and check if length <= radius
		difference = closest - center;
		if (glm::length(difference) < radius) {
			return true;
		}
	}
	for (auto const& pipeHead : blocks.pipeHeads) {
		glm::vec2 aabb_half_extents(pipeHead.scaleX / 2, pipeHead.scaleY / 2);
		glm::vec2 aabb_center(pipeHead.translateX + aabb_half_extents.x
			, pipeHead.translateY + aabb_half_extents.y
		);
		// Get difference vector between both centers
		glm::vec2 difference = center - aabb_center;
		glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
		// add clamped value to aabb_center and we get the value of box closest to circle
		glm::vec2 closest = aabb_center + clamped;
		// retrieve vector between center circle and closest point aabb and check if length <= radius
		difference = closest - center;
		if (glm::length(difference) < radius) {
			return true;
		}
	}
	return false;
}

int Collision::updateScore() {
	if (bird.lastLeftBehindPipe == -1) {
		auto pipe = blocks.forms.begin();

		if (pipe->translateX + pipe->scaleX < bird.form.translateX) {
			bird.lastLeftBehindPipe = pipe->id;
			return 1;
		}
	} else {
		for (auto const& pipe : blocks.forms) {
			if (pipe.id == (bird.lastLeftBehindPipe + 2) % 10) {
				// test if the next pipe after last left behind
				// is also passed
				if (pipe.translateX + pipe.scaleX < bird.form.translateX) {
					bird.lastLeftBehindPipe = pipe.id;  // vezi care e faca
					return 1;
				}
			}
		}
	}
	
	return 0;
}