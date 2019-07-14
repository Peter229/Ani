#include "player.h"

Player::Player() {

	minPos = glm::vec3(0.0f, 0.0f, 0.0f);
	maxPos = glm::vec3(1.0f, 2.0f, 1.0f);
	vel = glm::vec3(0.0f);
	pos = glm::vec3(4.0f, 10.0f, 4.0f);
	speed = 0.002;
	grounded = false;
}

Player::~Player() {

}

void Player::createPlayer() {

	glm::vec3 verta = minPos;
	glm::vec3 vertb = glm::vec3(minPos.x, minPos.y, minPos.z + maxPos.z);
	glm::vec3 vertc = glm::vec3(minPos.x + maxPos.x, minPos.y, minPos.z);
	glm::vec3 vertd = glm::vec3(minPos.x + maxPos.x, minPos.y, minPos.z + maxPos.z);
	glm::vec3 verte = glm::vec3(minPos.x, minPos.y + maxPos.y, minPos.z);
	glm::vec3 vertf = glm::vec3(minPos.x, minPos.y + maxPos.y, minPos.z + maxPos.z);
	glm::vec3 vertg = glm::vec3(minPos.x + maxPos.x, minPos.y + maxPos.y, minPos.z);
	glm::vec3 verth = glm::vec3(minPos.x + maxPos.x, minPos.y + maxPos.y, minPos.z + maxPos.z);

	glm::vec3 norma = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 normb = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 normc = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 normd = glm::vec3(-1.0f, 0.0f, 0.0f);
	glm::vec3 norme = glm::vec3(0.0f, -1.0f, 0.0f);
	glm::vec3 normf = glm::vec3(0.0f, 0.0f, -1.0f);


	glm::vec2 texa = glm::vec2(0.0f, 0.0f);
	glm::vec2 texb = glm::vec2(maxPos.x, 0.0f);
	glm::vec2 texc = glm::vec2(0.0f, maxPos.z);
	glm::vec2 texd = glm::vec2(maxPos.x, maxPos.z);
	glm::vec2 texe = glm::vec2(0.0f, maxPos.y);
	glm::vec2 texf = glm::vec2(maxPos.x, maxPos.y);
	glm::vec2 texg = glm::vec2(maxPos.z, maxPos.y);
	glm::vec2 texh = glm::vec2(maxPos.z, 0.0f);

	//Edge Bottom
	Vertex3D vert1 = { verta, norme, texa };
	Vertex3D vert2 = { vertb, norme, texc };
	Vertex3D vert3 = { vertd, norme, texd };
	Vertex3D vert4 = { verta, norme, texa };
	Vertex3D vert5 = { vertd, norme, texd };
	Vertex3D vert6 = { vertc, norme, texb };
	verts.push_back(vert1);
	verts.push_back(vert2);
	verts.push_back(vert3);
	verts.push_back(vert4);
	verts.push_back(vert5);
	verts.push_back(vert6);

	//Edge Top
	vert1 = { verte, normb, texb };
	vert2 = { verth, normb, texc };
	vert3 = { vertf, normb, texd };
	vert4 = { verte, normb, texb };
	vert5 = { vertg, normb, texa };
	vert6 = { verth, normb, texc };
	verts.push_back(vert1);
	verts.push_back(vert2);
	verts.push_back(vert3);
	verts.push_back(vert4);
	verts.push_back(vert5);
	verts.push_back(vert6);

	//Edge Right
	vert1 = { verta, normd, texa };
	vert2 = { verte, normd, texe };
	vert3 = { vertb, normd, texh };
	vert4 = { vertb, normd, texh };
	vert5 = { verte, normd, texe };
	vert6 = { vertf, normd, texg };
	verts.push_back(vert1);
	verts.push_back(vert2);
	verts.push_back(vert3);
	verts.push_back(vert4);
	verts.push_back(vert5);
	verts.push_back(vert6);

	//Edge Left
	vert1 = { vertc, norma, texh };
	vert2 = { vertd, norma, texa };
	vert3 = { vertg, norma, texg };
	vert4 = { vertd, norma, texa };
	vert5 = { verth, norma, texe };
	vert6 = { vertg, norma, texg };
	verts.push_back(vert1);
	verts.push_back(vert2);
	verts.push_back(vert3);
	verts.push_back(vert4);
	verts.push_back(vert5);
	verts.push_back(vert6);

	//Edge Front
	vert1 = { vertb, normc, texa };
	vert2 = { vertf, normc, texe };
	vert3 = { verth, normc, texf };
	vert4 = { vertb, normc, texa };
	vert5 = { verth, normc, texf };
	vert6 = { vertd, normc, texb };
	verts.push_back(vert1);
	verts.push_back(vert2);
	verts.push_back(vert3);
	verts.push_back(vert4);
	verts.push_back(vert5);
	verts.push_back(vert6);

	//Edge Back
	vert1 = { verta, normf, texb };
	vert2 = { vertg, normf, texe };
	vert3 = { verte, normf, texf };
	vert4 = { verta, normf, texb };
	vert5 = { vertc, normf, texa };
	vert6 = { vertg, normf, texe };
	verts.push_back(vert1);
	verts.push_back(vert2);
	verts.push_back(vert3);
	verts.push_back(vert4);
	verts.push_back(vert5);
	verts.push_back(vert6);

	genVAO();
}

void Player::render(Shader *shader) {

	glBindTexture(GL_TEXTURE_2D, Resource_Manager::getTexture("crosshair"));
	glBindVertexArray(VAO);
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, pos + glm::vec3(-0.5f, -2.0f, -0.5f));
	shader->setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, verts.size());
}

void Player::move(int key) {

	// 0 Up 1 Down 2 left 3 Right
	switch (key) {
	case 0:
		if (grounded) {
			vel += dir * speed;
		}
		break;
	case 1:
		if (grounded) {
			vel -= dir * speed;
		}
		break;
	case 2:
		if (grounded) {
			vel -= right * speed;
		}
		break;
	case 3:
		if (grounded) {
			vel += right * speed;
		}
		break;
	case 4:
		pos.y += speed;
		break;
	case 5:
		pos.y -= speed;
		break;
	case 6:
		if (grounded) {
			vel.y = 0.98f;
		}
		break;
	}
}

void Player::updateYaw(float offset) {

	yaw = offset;

	dir.x = cos(glm::radians(yaw));
	dir.y = 0;
	dir.z = sin(glm::radians(yaw));
	dir = glm::normalize(dir);
	right = glm::normalize(glm::cross(dir, glm::vec3(0.0f, 1.0f, 0.0f)));
}

void Player::checkCollision(levelCreator* level) {

	pos = level->playerCollision (pos, glm::vec3(1.0f, 2.0f, 1.0f), vel, &grounded, &vel);

	/*float closeDis = 0.5f;

	if (translate != glm::vec3(0.0f)) {
		pos = translate;
		std::cout << translate.x << " " << translate.y << " " << translate.z << "\n";
	}
	*/
	/*if (translate != glm::vec3(0.0f)) {
		std::cout << translate.x << "\n";
		if (abs(translate.x) < closeDis) {
			pos.x += vel.x + translate.x;
			std::cout << translate.x << "\n";
		}
		if (abs(translate.y) < closeDis) {
			pos.y += vel.y + translate.y;
		}
		if (abs(translate.z) < closeDis) {
			pos.z += vel.z + translate.z;
		}
		//std::cout << translate.x << " " << translate.y << " " << translate.z << "\n";
		//std::cout << pos.x << " " << pos.y << " " << pos.z << "\n";
	}
	else {
		pos += vel;
	}*/

	glm::vec3 airFriction = glm::vec3(1.01f);
	glm::vec3 groundFriction = glm::vec3(1.5f, 1.0f, 1.5f);

	if (grounded) {
		vel /= groundFriction;
	}
	else {
		vel /= airFriction;
	}
	float close = 0.002f;
	if (fabs(vel.x) < close) {
		vel.x = 0;
	}
	if (fabs(vel.z) < close) {
		vel.z = 0;
	}
	if (fabs(vel.y) < close) {
		vel.y = 0;
	}
}

void Player::setGrounded(bool grounded) {

	this->grounded = grounded;
}

void Player::cleanUp() {

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

glm::vec3 Player::getPos() {

	return pos;
}

void Player::setPos(glm::vec3 pos) {

	this->pos = pos;
}

void Player::genVAO() {

	cleanUp();

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