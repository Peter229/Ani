#include "hud.h"

hud::hud(glm::mat4 model, int windowWidth, int windowHeight) {
	//model = glm::translate(model, glm::vec3(-0.99f, 0.99f, 0.0));
	this->windowHeight = windowHeight;
	this->windowWidth = windowWidth;
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.006f * 9, 0.006f * 16, 1.0f));
	this->model = model;
	//genVAO();
}

void hud::render(Shader *shader) {
	if (VAO != 0) {
		glBindVertexArray(VAO);
		glBindTexture(GL_TEXTURE_2D, Resource_Manager::getTexture("font"));
		shader->setMat4("model", model);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glDrawArrays(GL_TRIANGLES, 0, hudVerts.size());
	}
	if (menuVAO != 0) {
		glBindVertexArray(menuVAO);
		glBindTexture(GL_TEXTURE_2D, Resource_Manager::getTexture("textures"));
		shader->setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, menuVerts.size());
	}
}

void hud::cleanUp() {
	if (VAO != 0 || VBO != 0) {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		VAO = 0;
		VBO = 0;
	}
	//glDeleteBuffers(1, &EBO);
}

void hud::cleanUpMenu() {
	if (menuVAO != 0 || menuVBO != 0) {
		glDeleteVertexArrays(1, &menuVAO);
		glDeleteBuffers(1, &menuVBO);
		menuVAO = 0;
		menuVBO = 0;
	}
}

void hud::drawTextures() {

	int amountOfTextures = (sizeOfTextureImage * sizeOfTextureImage) / (sizeOfTexture * sizeOfTexture);

	float posX = -18.0f;
	float posY = -5.0f;
	float gap = 1.0f;

	for (int i = 0; i < amountOfTextures; i++) {

		int texturePos = i;
		int imagesPerRow = sizeOfTextureImage / sizeOfTexture;
		int horiz = texturePos % imagesPerRow;
		int verti = floor(texturePos / imagesPerRow);
		float horizInImage = (float)horiz / imagesPerRow;
		float vertiInImage = (float)verti / imagesPerRow;
		float increment = (float)sizeOfUI / sizeOfTextureImage;

		Vertex2D v1 = { glm::vec2((-0.5f * scaleTexture) + posX + (i * gap), (-0.5f * scaleTexture) + posY), glm::vec2(horizInImage, vertiInImage) }; //Top left
		Vertex2D v2 = { glm::vec2((0.5f * scaleTexture) + posX + (i * gap), (0.5f * scaleTexture) + posY), glm::vec2(horizInImage + increment, vertiInImage - increment) }; //Bottom right
		Vertex2D v3 = { glm::vec2((0.5f * scaleTexture) + posX + (i * gap), (-0.5f * scaleTexture) + posY), glm::vec2(horizInImage + increment, vertiInImage) }; //Top right
		Vertex2D v4 = { glm::vec2((-0.5f * scaleTexture) + posX + (i * gap), (0.5f * scaleTexture) + posY), glm::vec2(horizInImage, vertiInImage - increment) }; //Bottom left
		
		menuVerts.push_back(v1);
		menuVerts.push_back(v3);
		menuVerts.push_back(v2);
		menuVerts.push_back(v1);
		menuVerts.push_back(v2);
		menuVerts.push_back(v4);
		std::cout << menuVerts[i*6].pos.x << " " << menuVerts[i*6].pos.y << "\n";
	}

	//std::cout << menuVerts[0].pos.x << " " << menuVerts[0].pos.y << "\n";

	cleanUpMenu();
	genMenuVAO();
}

int hud::checkCollision(double xpos, double ypos) {

	float hudPosX = -((windowWidth / 2.0f) - xpos); //* 0.006f * 9;
	float hudPosY = -((windowHeight / 2.0f)- ypos);// * 0.006f * 16;

	hudPosX = hudPosX * (0.006f * 9.0f);
	hudPosY = hudPosY * (0.006f * 16.0f);

	std::cout << hudPosX << " " << hudPosY << "\n";

	return 0;
}

void hud::drawCrosshair() {
	
	std::vector<Vertex2D> tempVerts;

	int crossHairPos = 128;
	int imagesPerRow = sizeOfFontImage / sizeOfUI;
	int horiz = crossHairPos % imagesPerRow;
	int verti = floor(crossHairPos / imagesPerRow);
	float horizInImage = (float)horiz / imagesPerRow;
	float vertiInImage = (float)verti / imagesPerRow;
	float increment = (float)sizeOfUI / sizeOfFontImage;

	Vertex2D v1 = { glm::vec2(-0.5f * crossHairSize, -0.5f * crossHairSize), glm::vec2(horizInImage, vertiInImage) }; //Top left
	Vertex2D v2 = { glm::vec2(0.5f * crossHairSize, 0.5f * crossHairSize), glm::vec2(horizInImage + increment, vertiInImage - increment) }; //Bottom right
	Vertex2D v3 = { glm::vec2(0.5f * crossHairSize, -0.5f * crossHairSize), glm::vec2(horizInImage + increment, vertiInImage) }; //Top right
	Vertex2D v4 = { glm::vec2(-0.5f * crossHairSize, 0.5f * crossHairSize), glm::vec2(horizInImage, vertiInImage - increment) }; //Bottom left

	tempVerts.push_back(v1);
	tempVerts.push_back(v3);
	tempVerts.push_back(v2);
	tempVerts.push_back(v1);
	tempVerts.push_back(v2);
	tempVerts.push_back(v4);

	hudParts.push_back(tempVerts);

	//cleanUp();
	//genVAO();
	compileHud();
}

void hud::drawString(std::string string, float posx, float posy) {
	std::vector<char> chars;
	std::vector<int> charToInt;

	std::vector<Vertex2D> tempVerts;

	for (int i = 0; i < string.length(); i++) {

		//std::vector<Vertex2D> tempVerts;

		//std::cout << i << "\n";
		chars.push_back(string.at(i));
		if ((int)chars[i] >= 97) {
			charToInt.push_back((int)chars[i] - 97); //Lower case
		}
		else if ((int)chars[i] >= 65) {
			charToInt.push_back((int)chars[i] - 40); //Upper case
		}
		else if ((int)chars[i] >= 48) { //Numbers
			charToInt.push_back((int)chars[i] + 3);
		}
		else { //Space
			charToInt.push_back((int)chars[i] + 30);
		}
		Vertex2D v1 = { glm::vec2(((posx + (i * 1.0f)) * scale) * 8, (posy * scale) * 16), glm::vec2((charToInt[i] % (int)(sizeOfFontImage / sizeOfFontLetterX)) * (0.5f / sizeOfFontLetterX), (1 - ((2.0f / sizeOfFontLetterY) * (floor(charToInt[i] / sizeOfFontLetterY))))) };
		Vertex2D v2 = { glm::vec2((((posx + 1.0f) + (i * 1.0f)) * scale) * 8, ((posy + 1.0f) * scale) * 16), glm::vec2(((charToInt[i] % (int)(sizeOfFontImage / sizeOfFontLetterX)) * (0.5f / sizeOfFontLetterX)) + (0.5f / sizeOfFontLetterX), (1 - ((2.0f / sizeOfFontLetterY) * (floor(charToInt[i] / sizeOfFontLetterY)))) - (sizeOfFontLetterY / sizeOfFontImage)) };
		Vertex2D v3 = { glm::vec2((((posx + 1.0f) + (i * 1.0f)) * scale) * 8, (posy * scale) * 16), glm::vec2(((charToInt[i] % (int)(sizeOfFontImage / sizeOfFontLetterX)) * (0.5f / sizeOfFontLetterX)) + (0.5f / sizeOfFontLetterX), (1 - ((2.0f / sizeOfFontLetterY) * (floor(charToInt[i] / sizeOfFontLetterY))))) };
		Vertex2D v4 = { glm::vec2(((posx + (i * 1.0f)) * scale) * 8, ((posy + 1.0f) * scale) * 16), glm::vec2((charToInt[i] % (int)(sizeOfFontImage / sizeOfFontLetterX)) * (0.5f / sizeOfFontLetterX), (1 - ((2.0f / sizeOfFontLetterY) * (floor(charToInt[i] / sizeOfFontLetterY)))) - (sizeOfFontLetterY / sizeOfFontImage)) };

		/*Vertex v1 = { glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f) };
		Vertex v2 = { glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 1.0f) };
		Vertex v3 = { glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 0.0f) };
		Vertex v4 = { glm::vec2(0.0f, 1.0f), glm::vec2(0.0f, 1.0f) };*/

		tempVerts.push_back(v1);
		tempVerts.push_back(v3);
		tempVerts.push_back(v2);
		tempVerts.push_back(v1);
		tempVerts.push_back(v2);
		tempVerts.push_back(v4);
	}

	hudParts.push_back(tempVerts);
	
	//cleanUp();
	//genVAO();
	compileHud();
}

void hud::genVAO() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex2D) * hudVerts.size(), &hudVerts[0], GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesPlane), indicesPlane, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void hud::genMenuVAO() {

	glGenVertexArrays(1, &menuVAO);
	glGenBuffers(1, &menuVBO);

	glBindVertexArray(menuVAO);

	glBindBuffer(GL_ARRAY_BUFFER, menuVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex2D) * menuVerts.size(), &menuVerts[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void hud::compileHud() {

	hudVerts.clear();

	cleanUp();

	for (int i = 0; i < hudParts.size(); i++) {

		hudVerts.insert(hudVerts.end(), hudParts[i].begin(), hudParts[i].end());
	}

	if (hudVerts.size() > 0) {
		genVAO();
	}
}

void hud::removeHudPart(int p) {

	//hudVerts.clear();

	//cleanUp();

	hudParts.erase(hudParts.begin() + p);

	/*std::vector<std::vector<Vertex2D>> tempHudParts;
	std::vector<Vertex2D> tempVerts;

	for (int i = 0; i < hudParts.size(); i++) {

		if (i == p) {
			//Do nothing
			
		}
		else {
			tempVerts.insert(tempVerts.end(), hudParts[i].begin(), hudParts[i].end());
			tempHudParts.push_back(tempVerts);
			tempVerts.clear();
			hudVerts.insert(hudVerts.end(), hudParts[i].begin(), hudParts[i].end());
		}
	}

	hudParts.clear();
	hudParts = tempHudParts;*/

	//cleanUp();
	//genVAO();
	compileHud();
}