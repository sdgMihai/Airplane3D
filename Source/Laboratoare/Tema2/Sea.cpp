#include "Sea.h"

void Sea::init(float translateX) {
	colorBody = glm::vec3(0, 0, 1);
	int disp = 1;
	float length = 0.99f;

	int numSegments = 32;
	// the first Circle
	float rBody = length / 2;
	float cxBody = 0.f;
	float cyBody = 0.f;
	float czBody = 0.f;
	std::vector<VertexFormat> verticesCircle1;
	verticesCircle1 = { VertexFormat(glm::vec3(cxBody, cyBody, czBody), colorBody) };

	for (int ii = 0; ii < numSegments; ii++) {
		float theta = 2.0f * (float)M_PI * float(ii) / float(numSegments);  // get the current angle

		float x = rBody * cosf(theta);  // calculate the x component
		float y = rBody * sinf(theta);  // calculate the y component

		VertexFormat aux(glm::vec3(x + cxBody, y + cyBody, czBody), colorBody);
		verticesCircle1.push_back(aux);
	}

	std::vector<unsigned short> indicesCircle1;

	for (unsigned short i = 0; i < numSegments - 1; ++i) {
		indicesCircle1.push_back(0);
		indicesCircle1.push_back(i + 1u);
		indicesCircle1.push_back(i + 2u);
	}

	indicesCircle1.push_back(0);
	indicesCircle1.push_back(numSegments);
	indicesCircle1.push_back(1);

	// the 2nd circle
	float cxC2 = 0.f;
	float cyC2 = 0.f;
	float czC2 = -2.f;
	std::vector<VertexFormat> verticesCircle2;
	verticesCircle2 = { VertexFormat(glm::vec3(cxC2, cyC2, czC2), colorBody) };

	for (int ii = 0; ii < numSegments; ii++) {
		float theta = 2.0f * (float)M_PI * float(ii) / float(numSegments);  // get the current angle

		float x = rBody * cosf(theta);  // calculate the x component
		float y = rBody * sinf(theta);  // calculate the y component

		VertexFormat aux(glm::vec3(x + cxC2, y + cyC2, czC2), colorBody);
		verticesCircle2.push_back(aux);
	}

	std::vector<unsigned short> indicesCircle2;

	for (unsigned short i = 0; i < numSegments - 1; ++i) {
		indicesCircle2.push_back(0 + verticesCircle1.size());
		indicesCircle2.push_back(i + 1u + verticesCircle1.size());
		indicesCircle2.push_back(i + 2u + verticesCircle1.size());
	}

	indicesCircle2.push_back(0 + verticesCircle1.size());
	indicesCircle2.push_back(numSegments + verticesCircle1.size());
	indicesCircle2.push_back(1 + verticesCircle1.size());
	// DONE connect circles
	std::vector<unsigned short> ext;
	for (unsigned short i = 0; i < numSegments - 1; ++i) {
		ext.push_back(i + 1);  // the 1st on the circle ext, 0th is the circle center
		ext.push_back(i + verticesCircle1.size() + 1);
		ext.push_back(i + verticesCircle1.size() + 2);

		ext.push_back(i + 1);  // the 1st on the circle ext, 0th is the circle center
		ext.push_back(i + verticesCircle1.size() + 2);
		ext.push_back(i + 2);
	}
	ext.push_back(numSegments);
	ext.push_back(numSegments + verticesCircle1.size());
	ext.push_back(numSegments + verticesCircle1.size() + 1);

	ext.push_back(numSegments);
	ext.push_back(numSegments + verticesCircle1.size() + 1);
	ext.push_back(1);

	indices = indicesCircle1;
	indices.insert(indices.begin(), indicesCircle2.begin(), indicesCircle2.end());
	indices.insert(indices.begin(), ext.begin(), ext.end());
	vertices = verticesCircle1;
	vertices.insert(vertices.end(), verticesCircle2.begin(), verticesCircle2.end());

	form.setTranslate(translateX, 0, 1.2f);
	form.setScale(1, 0.7f, 35.f);
}

void Sea::updateForm() {
	modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, form.getTranslate());
	modelMatrix = glm::scale(modelMatrix, form.getScale());
}

void Sea::move(float delta) {
	return;
	// movement in shader
}

void Sea::startMoving() {
	return;
	// movement in shader
}