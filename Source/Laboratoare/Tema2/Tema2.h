#pragma once
#include <Component/SimpleScene.h>
#include <Core/GPU/Mesh.h>
#include <vector>
#include "Plane.h"
#include "Sea.h"
#include "Clouds.h"
#include "Laboratoare/Laborator5/LabCamera.h"
#include "Obstacles.h"

enum class CameraType : bool { firstPerson, thirdPerson };

class Tema2 : public SimpleScene
{
public:
	Tema2();
	~Tema2();

	void Init() override;

	Mesh * CreateMesh(const char * name, const std::vector<VertexFormat> &vertices, const std::vector<unsigned short> &indices);

private:
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	void RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, int isSea, const glm::vec3 &color = glm::vec3(1));
	void DrawScene();

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;

	void toggleCamera();
	void updateCamera();

	Plane plane;
	Sea sea;
	std::vector<Sea> wave;
	Clouds clouds;
	Obstacles fuelReg;
	Obstacles obstacles;
	bool startGame, stopGame;
	// camera data
	glm::mat4 projectionMatrix;
	CameraType cameraType;
	Laborator::Camera *camera;
	bool renderCameraTarget;
	// please don't use these features unless you know what are you doing
	float fov;
	float left;
	float bottom;
};
