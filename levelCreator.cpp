#include "levelCreator.h"

levelCreator::levelCreator() {
	selectedBrushIndex = -1;
}

levelCreator::~levelCreator() {

}

void levelCreator::extendOct(glm::vec3 dir, glm::vec3 orig) {

	float t = rayPlaneIntersection(dir, orig, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	if (t > 0) {
		glm::vec3 pos = orig + (dir * t);
		pos.x = floor(pos.x);
		pos.y = floor(pos.y);
		pos.z = floor(pos.z);
		glm::vec3 pos2 = pos;
		glm::vec3 pos3 = pos;
		glm::vec3 pos4 = pos;
		pos2.x += 1.0f;
		pos2.z += 1.0f;
		pos3.x += 1.0f;
		pos4.z += 1.0f;
		for (int i = 0; i < verts.size(); i++) {
			if (pos == verts[i].pos || pos2 == verts[i].pos || pos3 == verts[i].pos || pos4 == verts[i].pos) {
				//verts[i].pos += glm::vec3(0.0f, 1.0f, 0.0f);
				//verts[i].normal = glm::vec3(0.3f, 0.3f, 0.4f);
			}
		}

		cleanUp();
		genVAO();
	}
}

void levelCreator::createBrush(glm::vec3 pos) {

	brushes.push_back(new Brush(pos));

	verts.insert(verts.end(), brushes.back()->getVerts()->begin(), brushes.back()->getVerts()->end());

	cleanUp();
	genVAO();
}

void levelCreator::createBrush(glm::vec3 pos, glm::vec3 add) {

	brushes.push_back(new Brush(pos, add));

	verts.insert(verts.end(), brushes.back()->getVerts()->begin(), brushes.back()->getVerts()->end());

	cleanUp();
	genVAO();
}

void levelCreator::findBrushes(glm::vec3 dir, glm::vec3 orig) {

	int brushIndex = -1;

	float t = -9.0f;
	float preBest = 99999.0f;

	for (int i = 0; i < brushes.size(); i++) {
		if (brushes[i]->rayAABB(orig, dir)) {
			t = brushes[i]->rayAABBf(orig, dir);
			if (t >= 0) {
				if (preBest > t) {
					preBest = t;
					brushIndex = i;
				}
			}
		}
	}

	if (brushIndex >= 0) {
		if (selectedBrushIndex == brushIndex) {
			brushes[selectedBrushIndex]->cleanUp();
			selectedBrushIndex = -1;
			//std::cout << "Brush deselected\n";
		}
		else {
			selectedBrushIndex = brushIndex;
			brushes[selectedBrushIndex]->cleanUp();
			brushes[selectedBrushIndex]->genVAO();
			//std::cout << "Brush selected\n";
		}
	}
	else {
		selectedBrushIndex = -1;
	}
}

void levelCreator::editBrush(int dir) {

	if (selectedBrushIndex >= 0) {

		if (dir != 60) {
			brushes[selectedBrushIndex]->moveBrush(dir);
			brushes[selectedBrushIndex]->cleanUp();
			brushes[selectedBrushIndex]->genVAO();
			verts.clear();

			for (int i = 0; i < brushes.size(); i++) {
				verts.insert(verts.end(), brushes[i]->getVerts()->begin(), brushes[i]->getVerts()->end());
			}

			cleanUp();
			genVAO();
		}
		else {
			brushes[selectedBrushIndex]->~Brush();
			delete brushes[selectedBrushIndex];
			brushes.erase(brushes.begin() + selectedBrushIndex);

			verts.clear();

			for (int i = 0; i < brushes.size(); i++) {
				verts.insert(verts.end(), brushes[i]->getVerts()->begin(), brushes[i]->getVerts()->end());
			}

			cleanUp();

			if (verts.size() > 0) {
				genVAO();
			}

			selectedBrushIndex = -1;
		}
	}
}

void levelCreator::rotateBrush(int axis, int dir) {

	if (selectedBrushIndex >= 0) {

		brushes[selectedBrushIndex]->rotateBrush(axis, dir);
		brushes[selectedBrushIndex]->cleanUp();
		brushes[selectedBrushIndex]->genVAO();
		verts.clear();

		for (int i = 0; i < brushes.size(); i++) {
			verts.insert(verts.end(), brushes[i]->getVerts()->begin(), brushes[i]->getVerts()->end());
		}

		cleanUp();
		genVAO();
	}
}

glm::vec3 levelCreator::playerCollision(glm::vec3 boxCenter, glm::vec3 boxHalfSize, glm::vec3 vel) {

	//for (int i = 0; i < brushes.size(); i++) {

		//brushes[i]->triAABB(boxCenter, boxHalfSize, vel, translation);
	//}

	CollisionPacket cp;
	cp.ellipsoidSpace = boxHalfSize;
	cp.w_Position = boxCenter;
	cp.w_Velocity = vel;

	glm::vec3 posF = Collision::CollisionSlide(cp, verts);

	return posF;
}

void levelCreator::findTri(const glm::vec3 &orig, const glm::vec3 &dir) {

	if (selectedBrushIndex >= 0) {
		if (brushes[selectedBrushIndex]->findTri(orig, dir)) {
			verts.clear();
			for (int i = 0; i < brushes.size(); i++) {
				verts.insert(verts.end(), brushes[i]->getVerts()->begin(), brushes[i]->getVerts()->end());
			}
			cleanUp();
			genVAO();
		}
	}
}

void levelCreator::findVertex(const glm::vec3 &orig, const glm::vec3 &dir, int move) {

	if (selectedBrushIndex >= 0) {
		if (brushes[selectedBrushIndex]->findVertex(orig, dir, move)) {
			verts.clear();
			for (int i = 0; i < brushes.size(); i++) {
				verts.insert(verts.end(), brushes[i]->getVerts()->begin(), brushes[i]->getVerts()->end());
			}
			cleanUp();
			genVAO();
		}
	}
}

void levelCreator::optimize() {

}

void levelCreator::cleanUp() {

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void levelCreator::render(Shader *shader) {

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Resource_Manager::getTexture("DCrosshair"));
	glBindVertexArray(VAO);
	glm::mat4 model = glm::mat4(1.0f);
	shader->setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, verts.size());

	//if (selectedBrushIndex >= 0) {

		//brushes[selectedBrushIndex]->render(shader);
	//}
}

void levelCreator::render(Shader *shader, unsigned int* depthMap) {

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Resource_Manager::getTexture("DCrosshair"));
	if (depthMap != 0) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, *depthMap);
	}
	glBindVertexArray(VAO);
	glm::mat4 model = glm::mat4(1.0f);
	shader->setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, verts.size());
}

void levelCreator::renderBrushSelection(Shader *shader) {
	if (selectedBrushIndex >= 0) {

		brushes[selectedBrushIndex]->render(shader);
	}
}

void levelCreator::genVAO() {

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex3D) * verts.size(), &verts[0], GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}