
#include "Tester.h"

Tester* Tester::tester = nullptr;

Tester::Tester() {
	out.open(".\\Source\\Laboratoare\\Tema1\\log.txt");
}

Tester::~Tester() {
	if (tester != nullptr) {
		delete tester;
		tester = nullptr;
	}
}

void Tester::printBlocks(const Blocks* input) {
	out << input->resolution.x << " and res.y : " << input->resolution.y << '\n';

	for (auto& elem : input->forms) {
		printPipe(&elem);
	}
	out << '\n';
}

void Tester::printBird(const Bird* bird) {
	out << "print Bird\n";
	printTransform(&bird->form);
	out << "fallSteps " << bird->fallSteps << ' ' << bird->riseSteps << '\n';
	out << '\n';
}

void Tester::printTransform(const Transform* transform){
	out << "Transform\n";
	out << "translateX " << transform->translateX << " ";
	out << "translateY " << transform->translateY << "  ";
	out << "scaleX " << transform->scaleX << " ";
	out << "scaleY " << transform->scaleY << " ";
	out << "angle " << transform->angle << "\n";
}

Tester* Tester::getTester() {
	if (tester == nullptr) {
		tester = new Tester();
	}

	return tester;
}

void Tester::printCollision(const Collision* collision) {
	out << "testing collision\n";
	printBird(&collision->bird);
	out << "block " << collision->blocks.forms.begin()->translateX << '\n';
	out << "collisions : \n" << (collision->testCollision() ? "true" : "false") << "\n\n";
}

void Tester::printPipe(const Pipe* pipe) {
	out << "Pipe\n";
	out << "translateX " << pipe->translateX << " ";
	out << "translateY " << pipe->translateY << "  ";
	out << "scaleX " << pipe->scaleX << " ";
	out << "scaleY " << pipe->scaleY << " ";
	out << "angle " << pipe->angle << "\n";
	out << "id " << pipe->id << "\n";
	out << "isShrinking " << (pipe->isShrinking == true ? "true" : "false") << '\n';
	out << "shrinkSteps " << pipe->shrinkSteps << '\n';
	out << "enlargeSteps " << pipe->enlargeSteps << '\n';
}