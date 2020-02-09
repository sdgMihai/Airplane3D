#include "Tema1.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>
#include "Tema1.h"
#include "Transform2D.h"
#include "Object2D.h"
#include "Bird.h"
#include "Transform.h"
#include "Collision.h"
#include "Tester.h"
#include "GameStatistics.h"

using namespace std;

Tema1::Tema1()
{
}

Tema1::~Tema1()
{
}

void Tema1::Init()
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

	startGame = false;
	stopGame = false;

	// Create the bird from buffer data and add it to mesh list
	// set position and starting scale
	bird.init();
	Mesh* bMesh1 = CreateMesh("body", bird.verticesBody, bird.indicesBody);
	AddMeshToList(bMesh1);

	Mesh* bMesh2 = CreateMesh("head", bird.verticesHead, bird.indicesHead);
	AddMeshToList(bMesh2);

	Mesh* bMesh3 = CreateMesh("beak", bird.verticesBeak, bird.indicesBeak);
	AddMeshToList(bMesh3);

	Mesh* bMesh4 = CreateMesh("wing", bird.verticesWing, bird.indicesWing);
	AddMeshToList(bMesh4);

	Mesh* bMesh5 = CreateMesh("eye", bird.verticesEye, bird.indicesEye);
	AddMeshToList(bMesh5);

	pipeNames = { "square0", "square1" };

	glm::vec3 leftBottomCorner = glm::vec3(0.001, 0.001, 0);
	blocks.squareSide = 1;
	Mesh* square0 = Object2D::CreateSquare(pipeNames[0].c_str(), leftBottomCorner, blocks.squareSide, glm::vec3(0.6f, 1, 0.4f), true);
	AddMeshToList(square0);

	Mesh* square1 = Object2D::CreateSquare(pipeNames[1].c_str(), leftBottomCorner, blocks.squareSide, glm::vec3(1, 0.4f, 0), true);
	AddMeshToList(square1);

	glm::ivec2 resolution = window->GetResolution();
	blocks.setResolution(resolution);
	blocks.init();

	// start Collision class
	Collision::getCollision(bird, blocks);

	GameStatistics::getGameStatistics()->printStartingMessage();
}

Mesh* Tema1::CreateMesh(const char *name, const std::vector<VertexFormat> &vertices, const std::vector<unsigned short> &indices)
{
	unsigned int VAO = 0;
	// DONE: Create the VAO and bind it
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// DONE: Create the VBO and bind it
	unsigned int VBO = 0;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// DONE: Send vertices data into the VBO buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	// DONE: Crete the IBO and bind it
	unsigned int IBO = 0;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	// DONE: Send indices data into the IBO buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

	// ========================================================================
	// This section describes how the GPU Shader Vertex Shader program receives data
	// It will be learned later, when GLSL shaders will be introduced
	// For the moment just think that each property value from our vertex format needs to be send to a certain channel
	// in order to know how to receive it in the GLSL vertex shader

	// set vertex position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

	// set vertex normal attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

	// set texture coordinate attribute
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

	// set vertex color attribute
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
	// ========================================================================

	// DONE: Unbind the VAO
	glBindVertexArray(0);

	// Check for OpenGL errors
	CheckOpenGLError();

	// Mesh information is saved into a Mesh object
	meshes[name] = new Mesh(name);
	meshes[name]->InitFromBuffer(VAO, static_cast<unsigned short>(indices.size()));
	return meshes[name];
}

// 2D visualization matrix
glm::mat3 Tema1::VisualizationTransf2D(const LogicSpace & logicSpace, const ViewportSpace & viewSpace)
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
glm::mat3 Tema1::VisualizationTransf2DUnif(const LogicSpace & logicSpace, const ViewportSpace & viewSpace)
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

void Tema1::SetViewportArea(const ViewportSpace & viewSpace, glm::vec3 colorColor, bool clear)
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

void Tema1::FrameStart()
{
	// Clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void Tema1::Update(float deltaTimeSeconds)
{	
	glm::ivec2 resolution = window->GetResolution();
	blocks.setResolution(resolution);

	// Sets the screen area where to draw - the left half of the window
	viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
	SetViewportArea(viewSpace, glm::vec3(0.4f, 1, 1), true);

	// Compute the 2D visualization matrix
	visMatrix = glm::mat3(1);
	visMatrix *= VisualizationTransf2DUnif(logicSpace, viewSpace);

	DrawScene(visMatrix);

	if (!stopGame) {
		bird.move(deltaTimeSeconds);
		if (startGame) {
			blocks.move(deltaTimeSeconds);
			GameStatistics::getGameStatistics()->update();
		}
	}

	// the bird is getting out of image or colliding a pipe
	if (Collision::getCollision()->testCollision() && !stopGame) {
		stopGame = true;
		GameStatistics::getGameStatistics()->printEndingMessage();
	}
}

void Tema1::FrameEnd()
{

}

void Tema1::DrawScene(glm::mat3 visMatrix)
{
	modelMatrix = glm::mat3(1);

	// the wing
	modelMatrix = Transform2D::Translate(bird.form.translateX, bird.form.translateY);
	modelMatrix *= Transform2D::Rotate(bird.getAngle());
	modelMatrix *= Transform2D::Scale(bird.form.scaleX, bird.form.scaleY);
	RenderMesh2D(meshes["wing"], shaders["VertexColor"], modelMatrix);

	// the body
	modelMatrix = Transform2D::Translate(bird.form.translateX, bird.form.translateY);
	modelMatrix *= Transform2D::Rotate(bird.getAngle());
	modelMatrix *= Transform2D::Scale(bird.form.scaleX, bird.form.scaleY);
	RenderMesh2D(meshes["body"], shaders["VertexColor"], modelMatrix);

	// the eye
	modelMatrix = Transform2D::Translate(bird.form.translateX, bird.form.translateY);
	modelMatrix *= Transform2D::Rotate(bird.getAngle());
	modelMatrix *= Transform2D::Scale(bird.form.scaleX, bird.form.scaleY);
	RenderMesh2D(meshes["eye"], shaders["VertexColor"], modelMatrix);

	// the head
	modelMatrix = Transform2D::Translate(bird.form.translateX, bird.form.translateY);
	modelMatrix *= Transform2D::Rotate(bird.getAngle());
	modelMatrix *= Transform2D::Scale(bird.form.scaleX, bird.form.scaleY);
	RenderMesh2D(meshes["head"], shaders["VertexColor"], modelMatrix);

	// the beak
	modelMatrix = Transform2D::Translate(bird.form.translateX, bird.form.translateY);
	modelMatrix *= Transform2D::Rotate(bird.getAngle());
	modelMatrix *= Transform2D::Scale(bird.form.scaleX, bird.form.scaleY);
	RenderMesh2D(meshes["beak"], shaders["VertexColor"], modelMatrix);

	if (startGame) {
		int cnt = 0;
		// draw blocks
		auto pipeIt = blocks.pipeHeads.begin();
		for (auto const& form : blocks.getForms()) {
			modelMatrix = Transform2D::Translate(form.translateX, form.translateY);
			modelMatrix *= Transform2D::Scale(form.scaleX, form.scaleY);
			RenderMesh2D(meshes[pipeNames[cnt].c_str()], shaders["VertexColor"], modelMatrix);

			Transform& pipe = *pipeIt;
			modelMatrix = Transform2D::Translate(pipe.translateX, pipe.translateY);
			modelMatrix *= Transform2D::Scale(pipe.scaleX, pipe.scaleY);
			RenderMesh2D(meshes[pipeNames[cnt].c_str()], shaders["VertexColor"], modelMatrix);
			cnt = (cnt + 1) % pipeNames.size();
			pipeIt++;
		}
	}
}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{
	if (window->KeyHold(GLFW_KEY_SPACE)) {
		bird.startMoving();
		if (!stopGame) {
			startGame = true;
		}
	}
}

void Tema1::OnKeyPress(int key, int mods)
{

}

void Tema1::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}