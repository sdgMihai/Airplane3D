#include "Blocks.h"
#include <cstdlib>
#include <time.h>       /* time */
#include <iostream>
#include "Tester.h"
#include "GameStatistics.h"


void Blocks::init() {
	int step = channel + 500;  // size of a block and space between two adj. blocks
	float sumOfRandomheights = 190 * 2;  // height of upper and lower block
	width = 130;
	lastMade = 0;
	lastId = -1;

	createAndAddBlock();
	createAndAddBlock();
}

void Blocks::createAndAddBlock() {
	// create and add to forms
	lastId = (lastId + 1) % 10;
	// lastMade boolean meaning upper or lower pipe need to be done
	if (lastMade == 0) {
		srand(time(NULL));
		float height1 = (float)(rand() % (resolution.y - channel));
		forms.push_back(Pipe(resolution.x - 1 + width / 3.f, resolution.y - height1, width / 3.f, height1, 0.f, lastId));
		lastMade = 1;
		pipeHeads.push_back(Transform((float)resolution.x, resolution.y - height1, width, pipeHeadHeight, 0.f));
	}
	else {
		forms.push_back(
			Pipe(resolution.x - 1.f + width / 3.f, 0, width / 3.f, resolution.y - forms.back().scaleY  - channel, 0.f, lastId)
		);
		pipeHeads.push_back(Transform(resolution.x - 1.f, forms.back().scaleY - pipeHeadHeight, width, pipeHeadHeight, 0.f));
		lastMade = 0;
	}
}

void Blocks::destroyBlock() {
	forms.pop_front();
	pipeHeads.pop_front();
}

void Blocks::move(float deltaTimeSeconds) {
	for (auto& elem : forms) {
		elem.translateX -= 80 * deltaTimeSeconds * (GameStatistics::getGameStatistics()->getScore() + 1);
		if (elem.isShrinking == true) {
			elem.shrink();
		} else {
			elem.enlarge();
		}
	}
	bool cnt = 0;
	auto& pipeIt = forms.begin();
	for (auto& elem : pipeHeads) {
		elem.translateX -= 80 * deltaTimeSeconds * (GameStatistics::getGameStatistics()->getScore() + 1);
		if (cnt == 0) {
			// pipeHead must be moved according to body pipe
			elem.translateY = pipeIt->translateY;
		} else {
			elem.translateY = pipeIt->scaleY - pipeHeadHeight;
		}
		cnt = !cnt;
		pipeIt++;
	}
	checkAndCreate();
}

void Blocks::checkAndCreate() {
	auto pipeIt = pipeHeads.begin();
	for (auto it = forms.begin(); it != forms.end() && pipeIt != pipeHeads.end(); ) {
		Pipe& form = *it;
		Transform& pipeHead = *pipeIt;
		if (form.translateX + squareSide * form.scaleX <= 0
			&& pipeHead.translateX + squareSide * pipeHead.scaleX <= 0) {
			// destroy the block and make new one at the right end
			// aka change the translateX component
			createAndAddBlock();
			createAndAddBlock();
			destroyBlock();
			destroyBlock();
			// this is a perfect patch because the list is very small
			it = forms.begin();
			pipeIt = pipeHeads.begin();
		}
		else {
			it++;
			pipeIt++;
		}
	}

	// there is room for two blocks
	if (forms.back().translateX + squareSide * forms.back().scaleX <= resolution.x / 2) {
		createAndAddBlock();
		createAndAddBlock();
	}
}

unsigned int Blocks::getNr() {
	return forms.size();
}

void Blocks::setResolution(glm::ivec2 aux) {
	resolution = aux;
}