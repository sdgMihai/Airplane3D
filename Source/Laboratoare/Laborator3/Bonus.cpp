#include "Bonus.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>
#include "Bonus.h"
#include "Transform2D.h"
#include "Object2D.h"

using namespace std;

Bonus::Bonus()
{
}

Bonus::~Bonus()
{
}

void Bonus::Init()
{
	auto camera = GetSceneCamera();
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	logicSpace.x = 0;		// logic x
	logicSpace.y = 0;		// logic y
	logicSpace.width = 4;	// logic width
	logicSpace.height = 4;	// logic height

	glm::vec3 corner = glm::vec3(0.001, 0.001, 0);
	length = 0.99f;

	Mesh* square1 = Object2D::CreateSquare("square1", corner, length, glm::vec3(1, 0, 0));
	AddMeshToList(square1);


	// initialize tx and ty (the translation steps)
	translateX = 0;
	translateY = 0;

	// initialize angularStep
	angularStep = 0;
}

// 2D visualization matrix
glm::mat3 Bonus::VisualizationTransf2D(const LogicSpace & logicSpace, const ViewportSpace & viewSpace)
{
	float sx, sy, tx, ty;
	sx = viewSpace.width / logicSpace.width;
	sy = viewSpace.height / logicSpace.height;
	tx = viewSpace.x - sx * logicSpace.x;
	ty = viewSpace.y - sy * logicSpace.y;

	return glm::transpose(glm::mat3(
		sx, 0.0f, tx,
		0.0f, sy, ty,
		0.0f, 0.0f, 1.0f));
}

// uniform 2D visualization matrix (same scale factor on x and y axes)
glm::mat3 Bonus::VisualizationTransf2DUnif(const LogicSpace & logicSpace, const ViewportSpace & viewSpace)
{
	float sx, sy, tx, ty, smin;
	sx = viewSpace.width / logicSpace.width;
	sy = viewSpace.height / logicSpace.height;
	if (sx < sy)
		smin = sx;
	else
		smin = sy;
	tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * logicSpace.width) / 2;
	ty = viewSpace.y - smin * logicSpace.y + (viewSpace.height - smin * logicSpace.height) / 2;

	return glm::transpose(glm::mat3(
		smin, 0.0f, tx,
		0.0f, smin, ty,
		0.0f, 0.0f, 1.0f));
}

void Bonus::SetViewportArea(const ViewportSpace & viewSpace, glm::vec3 colorColor, bool clear)
{
	glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

	glEnable(GL_SCISSOR_TEST);
	glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

	// Clears the color buffer (using the previously set color) and depth buffer
	glClearColor(colorColor.r, colorColor.g, colorColor.b, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_SCISSOR_TEST);

	GetSceneCamera()->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width), (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.1f, 400);
	GetSceneCamera()->Update();
}

void Bonus::FrameStart()
{
	// Clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);

}

void Bonus::Update(float deltaTimeSeconds)
{
	modelMatrix = glm::mat3(1);
	translateX -= deltaTimeSeconds * 10;
	//translateY += deltaTimeSeconds * 10;
	///modelMatrix = Transform2D::Translate(1, 0);
	modelMatrix *= Transform2D::Translate(translateX, translateY);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

	//modelMatrix = Transform2D::Translate(3, 0);
	modelMatrix *= Transform2D::Translate(10, translateY);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

	modelMatrix = Transform2D::Translate(2, 1.5);
	//modelMatrix *= Transform2D::Translate(translateX, translateY);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

	modelMatrix = Transform2D::Translate(1, 1);
	//modelMatrix *= Transform2D::Translate(translateX, translateY);
	modelMatrix *= Transform2D::Scale(3, 0.5);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);
}

void Bonus::FrameEnd()
{

}

void Bonus::DrawScene(glm::mat3 visMatrix)
{
	modelMatrix = visMatrix * Transform2D::Translate(1, 0);
	//modelMatrix *= Transform2D::Translate(translateX, translateY);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

	modelMatrix = visMatrix * Transform2D::Translate(3, 0);
	//modelMatrix *= Transform2D::Translate(translateX, translateY);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

	modelMatrix = visMatrix * Transform2D::Translate(2, 1.5);
	//modelMatrix *= Transform2D::Translate(translateX, translateY);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

	modelMatrix = visMatrix * Transform2D::Translate(1, 1);
	//modelMatrix *= Transform2D::Translate(translateX, translateY);
	modelMatrix *= Transform2D::Scale(3, 0.5);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

}

void Bonus::OnInputUpdate(float deltaTime, int mods)
{
	//TODO move the logic window with W, A, S, D (up, left, down, right)
	//TODO zoom in and zoom out logic window with Z and X
	if (window->KeyHold(GLFW_KEY_W)) {
		this->logicSpace.y += deltaTime * 0.25f;
	}
	if (window->KeyHold(GLFW_KEY_S)) {
		this->logicSpace.y += 0.25f * deltaTime;
	}
	if (window->KeyHold(GLFW_KEY_A)) {
		this->logicSpace.x += 0.25f * deltaTime;
	}
	if (window->KeyHold(GLFW_KEY_D)) {
		this->logicSpace.x -= 0.25f * deltaTime;
	}
	if (window->KeyHold(GLFW_KEY_Z)) {
		this->logicSpace.height -= 0.25f * deltaTime;
		this->logicSpace.width -= 0.25f * deltaTime;
	}
	if (window->KeyHold(GLFW_KEY_X)) {
		this->logicSpace.height += 0.25f * deltaTime;
		this->logicSpace.width += 0.25f * deltaTime;
	}
}

void Bonus::OnKeyPress(int key, int mods)
{

}

void Bonus::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Bonus::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void Bonus::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Bonus::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Bonus::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}