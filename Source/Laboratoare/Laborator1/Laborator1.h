#pragma once

#include <Component/SimpleScene.h>

struct Position {
	double x;
	double y;
	double z;
};

class Laborator1 : public SimpleScene
{
	public:
		Laborator1();
		~Laborator1();

		void Init() override;

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;

		int r = 0;
		int g = 0; 
		int b = 1;

		std::string tri[3];
		int it = 0;
		//Position pos(1, 0.5, 1);
		float x = 1;
		float y = 0.5f;
		float z = 1;

		bool isJumping;
		double maxHeight = 1.5;
		double isFalling;
		const int nrJumps = 3;
		int jumps = nrJumps;
		
};
