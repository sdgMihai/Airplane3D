#include "Tema2.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>
#include "Tester.h"
#include "GameStatistics3D.h"
#include "Collision3D.h"

using namespace std;

Tema2::Tema2()
{
}

Tema2::~Tema2()
{
}

void Tema2::Init()
{
	renderCameraTarget = false;

	camera = new Laborator::Camera();
	camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0.3, 1.8, 0), glm::vec3(0, 1, 0));
	cameraType = CameraType::firstPerson;
	
	plane.init();
	
	sea.init(0);
	float start = -6.f;
	wave.resize(20);
	for (int i = 0; i < wave.size(); ++i) {
		wave[i].init(start);
		start += 0.9f;
	}

	obstacles.init(glm::vec3(1, 0, 0), 2, 2);
	fuelReg.init(glm::vec3(0.251f, 0.87f, 0.815f), 24, 3);

	GameStatistics3D::getGameStatistics()->init();
	Collision3D::getCollision3D(plane, obstacles, fuelReg);

	clouds.init(glm::vec3(0, 0, 1));
	// Tester::getTester()->printForm(*clouds.forms.begin());

	CreateMesh("cube", plane.verticesBody, plane.indicesBody);
	CreateMesh("cylinder", sea.vertices, sea.indices);

	startGame = false;
	stopGame = false;

	// Create a shader program for drawing face polygon with the color of the normal
	{
		Shader *shader = new Shader("ShaderTema2");
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
	// Create a shader program for drawing face polygon with the color of the normal
	{
		Shader *shader2 = new Shader("ShaderTema2-2");
		shader2->AddShader("Source/Laboratoare/Tema2/Shaders/VertexShader2.glsl", GL_VERTEX_SHADER);
		shader2->AddShader("Source/Laboratoare/Tema2/Shaders/FragmentShader2.glsl", GL_FRAGMENT_SHADER);
		shader2->CreateAndLink();
		shaders[shader2->GetName()] = shader2;
	}

	fov = RADIANS(60);
	left = bottom = -3.f;

	// this app shouldn't use perspective, press button 'o'
	projectionMatrix = glm::perspective(fov, window->props.aspectRatio, 0.01f, 200.0f);
}

Mesh* Tema2::CreateMesh(const char *name, const std::vector<VertexFormat> &vertices, const std::vector<unsigned short> &indices)
{
	unsigned int VAO = 0;
	// DONE: Create the VAO and bind it
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// DONE: Create the VBO and bind it
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// DONE: Send vertices data into the VBO buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	// DONE: Crete the IBO and bind it
	unsigned int IBO;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	// DONE: Send indices data into the IBO buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

	// ========================================================================
	// This section describes how the GPU Shader Vertex Shader program receives data

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

	// Unbind the VAO
	glBindVertexArray(0);

	// Check for OpenGL errors
	CheckOpenGLError();

	// Mesh information is saved into a Mesh object
	meshes[name] = new Mesh(name);
	meshes[name]->InitFromBuffer(VAO, static_cast<unsigned short>(indices.size()));
	meshes[name]->vertices = vertices;
	meshes[name]->indices = indices;
	return meshes[name];
}


void Tema2::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 1, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::Update(float deltaTimeSeconds)
{
	// Process camera information.
	updateCamera();
	// render the objects.
	DrawScene();

	if (!stopGame) {
		// update the Model Matrix for plane, sea and gameStats.
		plane.updateForm();
		sea.updateForm();
		for (int i = 0; i < wave.size(); ++i) {
			wave[i].updateForm();
		}
		GameStatistics3D::getGameStatistics()->updateModelMatrix();
		
		// await the plane to enter scene then proceed to gaming.
		plane.move(deltaTimeSeconds);
		startGame = GameStatistics3D::getGameStatistics()->isGameReady(plane.formBody.translateY, startGame);
		Tester::getTester()->verifyApproach(plane);
		if (startGame) {
			GameStatistics3D::getGameStatistics()->update(deltaTimeSeconds);
			// the clous and the fuelReg modelMatrix
			// they are updated in the DrawScene() function, because they're a vector of elements.
			
			// move the rest of objects.
			clouds.move(deltaTimeSeconds);
			obstacles.move(deltaTimeSeconds);
			fuelReg.move(deltaTimeSeconds);

			// process information for frame
			Collision3D::getCollision3D()->testAndApplyCollision();
			if (GameStatistics3D::getGameStatistics()->isGameOver()) {
				stopGame = true;
			}
		}
	}
}

void Tema2::DrawScene() {
	
	/* Here I draw the plane, the sea and the game's Stats(fuel and lives).
	*/

	// the plane
	// the plane's body
	RenderSimpleMesh(meshes["cube"], shaders["VertexNormal"], plane.modelMatrixBody, 0);
	// the 1st blade and soon the 2nd mixed
	RenderSimpleMesh(meshes["cube"], shaders["VertexNormal"], plane.modelMatrixB1, 0);
	// the 2nd blade
	// RenderSimpleMesh(meshes["cube"], shaders["VertexNormal"], plane.modelMatrixB2, 0);
	// the 1st wing
	RenderSimpleMesh(meshes["cube"], shaders["VertexNormal"], plane.modelMatrixW1, 0);
	// the 2nd wing
	//RenderSimpleMesh(meshes["cube"], shaders["VertexNormal"], plane.modelMatrixW2, 0);
		
	// the sea
	RenderSimpleMesh(meshes["cylinder"], shaders["ShaderTema2"], sea.modelMatrix, 1, sea.colorBody);
	for (int i = 0; i < wave.size(); i += 2) {
		RenderSimpleMesh(meshes["cylinder"], shaders["ShaderTema2-2"], wave[i].modelMatrix, 1, wave[i].colorBody);
		RenderSimpleMesh(meshes["cylinder"], shaders["ShaderTema2"], wave[i + 1].modelMatrix, 1, wave[i + 1].colorBody);
	}

	/* Here I draw the gameStats.
	*/
	// the fuel
	RenderSimpleMesh(meshes["cube"]
		, shaders["ShaderTema2"]
		, GameStatistics3D::getGameStatistics()->modelMatrixFront
		, 0
		, GameStatistics3D::getGameStatistics()->colorFront);
	RenderSimpleMesh(meshes["cube"]
		, shaders["ShaderTema2"]
		, GameStatistics3D::getGameStatistics()->modelMatrixBack
		, 0
		, GameStatistics3D::getGameStatistics()->colorBack);
	// the hearts
	for (const auto& elem : GameStatistics3D::getGameStatistics()->modelMatrixHeart) {
		RenderSimpleMesh(meshes["cube"], shaders["VertexNormal"], elem, 0);
	}

	if (startGame) { // the game has started

		// the clouds
		for (const auto& elem : clouds.forms) {
			clouds.updateForm(elem);
			// Tester::getTester()->printForm(elem);
			RenderSimpleMesh(meshes["cube"], shaders["VertexNormal"], clouds.modelMatrix, 0);
		}

		// the obstacles
		for (const auto& elem : obstacles.forms) {
			obstacles.updateForm(elem);
			// Tester::getTester()->printForm(elem);
			RenderSimpleMesh(meshes["cube"], shaders["ShaderTema2"], obstacles.modelMatrix, 0, obstacles.color);
		}

		// the fuelRegenarators
		for (const auto& elem : fuelReg.forms) {
			fuelReg.updateForm(elem);
			// Tester::getTester()->printForm(elem);
			RenderSimpleMesh(meshes["cube"], shaders["ShaderTema2"], fuelReg.modelMatrix, 0, fuelReg.color);
		}

		// Render the camera target. Useful for understanding where is the rotation point in Third-person camera movement
		if (renderCameraTarget)
		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
			RenderMesh(meshes["sphere"], shaders["VertexNormal"], modelMatrix);
		}
	}

}

void Tema2::FrameEnd()
{
	// DrawCoordinatSystem();
	DrawCoordinatSystem(camera->GetViewMatrix(), projectionMatrix);
}

void Tema2::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, int isSea, const glm::vec3 &color)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	// DONE : get shader location for uniform mat4 "Model"
	int location = glGetUniformLocation(shader->program, "Model");
	// DONE : set shader uniform "Model" to modelMatrix
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	// DONE : get shader location for uniform mat4 "View"
	int locationView = glGetUniformLocation(shader->program, "View");
	// DONE : set shader uniform "View" to viewMatrix
	// as in lab > 5
	//glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
	// but here i need to use the personal implementation
	glm::mat4 viewMatrix = camera->GetViewMatrix();
	glUniformMatrix4fv(locationView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	// DONE : get shader location for uniform mat4 "Projection"
	int locationProjection = glGetUniformLocation(shader->program, "Projection");
	// DONE : set shader uniform "Projection" to projectionMatrix
	// same as the viewMatrix
	//glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
	glUniformMatrix4fv(locationProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	if (isSea) {
		//// BONUS var time
		float myTime = Engine::GetElapsedTime();
		int locationTime = glGetUniformLocation(shader->program, "myTime");
		glUniform1f(locationTime, myTime);
	}
	// send isSea so just the sea will have waves
	int locationIsSea = glGetUniformLocation(shader->program, "isSea");
	glUniform1i(locationIsSea, isSea);
	
	// send thisColor
	int locationThisColor = glGetUniformLocation(shader->program, "thisColor");
	glUniform3fv(locationThisColor, 1, glm::value_ptr(color));
	

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

// Documentation for the input functions can be found in: "/Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h

void Tema2::OnInputUpdate(float deltaTime, int mods)
{
	// add key press event
	// move the camera only if MOUSE_RIGHT button is pressed
	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		float cameraSpeed = 2.0f;

		if (window->KeyHold(GLFW_KEY_W)) {
			// DONE : translate the camera forward
			camera->TranslateForward(cameraSpeed * deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_A)) {
			// DONE : translate the camera to the left
			camera->TranslateRight(-1 * cameraSpeed * deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_S)) {
			// DONE : translate the camera backwards
			camera->TranslateForward(-1 * cameraSpeed * deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_D)) {
			// DONE : translate the camera to the right
			camera->TranslateRight(cameraSpeed * deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_Q)) {
			// DONE : translate the camera down
			camera->TranslateUpword(-1 * cameraSpeed * deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_E)) {
			// DONE : translate the camera up
			camera->TranslateUpword(cameraSpeed * deltaTime);
		}
	}

	if (window->KeyHold(GLFW_KEY_O)) {
		projectionMatrix = glm::perspective(fov, window->props.aspectRatio, 0.01f, 200.0f);
	}
	if (window->KeyHold(GLFW_KEY_P)) {
		projectionMatrix = glm::ortho(left, 3.f, -3.f, 3.f, 0.01f, 200.0f);
	}

	if (window->KeyHold(GLFW_KEY_K)) {
		fov++;
		projectionMatrix = glm::perspective(fov, window->props.aspectRatio, 0.01f, 200.0f);
	}
	if (window->KeyHold(GLFW_KEY_L)) {
		fov--;
		projectionMatrix = glm::perspective(fov, window->props.aspectRatio, 0.01f, 200.0f);
	}

	if (window->KeyHold(GLFW_KEY_H)) {
		left++;
		projectionMatrix = glm::ortho(left, 3.f, -3.f, 3.f, 0.01f, 200.0f);
	}
	if (window->KeyHold(GLFW_KEY_J)) {
		bottom++;
		projectionMatrix = glm::ortho(left, 3.f, bottom, 3.f, 0.01f, 200.0f);
	}
}

void Tema2::OnKeyPress(int key, int mods)
{
	// add key press event
	if (key == GLFW_KEY_T)
	{
		renderCameraTarget = !renderCameraTarget;
	}
	if (key == GLFW_KEY_C)
	{
		toggleCamera();
	}
}

void Tema2::toggleCamera() {
	
	cameraType = ((cameraType == CameraType::firstPerson) ?
		CameraType::thirdPerson : CameraType::firstPerson
		);
	if (cameraType == CameraType::firstPerson) {
		camera->RotateFirstPerson_OY(RADIANS(-90));
	} else {
		camera->RotateFirstPerson_OY(RADIANS(90));
	}
}

void Tema2::updateCamera() {
	if (cameraType == CameraType::firstPerson) {
		camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0.3, 1.8, 0), glm::vec3(0, 1, 0));
	} else {
		// move with the plane
		glm::vec3 pos(plane.formBody.translateX - 0.3f, plane.formBody.translateY + 0.3f, plane.formBody.translateZ);
		camera->Set(pos, glm::vec3(100, 1.8f, 0), glm::vec3(0, 1, 0));
	}
}

void Tema2::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
	if (!stopGame) {
		plane.startMoving(deltaY / 1.f, mouseY / 1.f);
		// Tester::getTester()->startMoving(plane);
	}
	// add mouse move event

	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		float sensivityOX = 0.001f;
		float sensivityOY = 0.001f;

		if (window->GetSpecialKeyState() == 0) {
			renderCameraTarget = false;
			// DONE : rotate the camera in First-person mode around OX and OY using deltaX and deltaY
			// use the sensitivity variables for setting up the rotation speed
			camera->RotateFirstPerson_OY(sensivityOX * deltaX);  // mouse se muta pe oY mesei, dar e rotatie in jurul ox pe ecran
			camera->RotateFirstPerson_OX(sensivityOY * deltaY);
		}

		if (window->GetSpecialKeyState() && GLFW_MOD_CONTROL) {
			renderCameraTarget = true;
			// DONE : rotate the camera in Third-person mode around OX and OY using deltaX and deltaY
			// use the sensitivity variables for setting up the rotation speed
			camera->RotateThirdPerson_OY(sensivityOX * deltaX);
			camera->RotateThirdPerson_OX(sensivityOY * deltaY);
		}

	}
}

void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema2::OnWindowResize(int width, int height)
{
}
