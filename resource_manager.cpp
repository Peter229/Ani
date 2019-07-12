#include "resource_manager.h"

static std::map<std::string, unsigned int> textures;
static std::map<std::string, unsigned int> vertexArrays;
static std::map<std::string, unsigned int> buffers;
static std::map<std::string, unsigned int> vertexs;

void Resource_Manager::clearData() {

	for (auto iter : textures) {
		glDeleteTextures(1, &iter.second);
	}

	for (auto iter : vertexArrays) {
		glDeleteVertexArrays(1, &iter.second);
	}

	for (auto iter : buffers) {
		glDeleteBuffers(1, &iter.second);
	}
}

unsigned int Resource_Manager::getTexture(std::string name) {

	return textures[name];
}

void Resource_Manager::loadTexture(const GLchar *file, std::string name, int flip) {

	textures[name] = loadTextureFromFile(file, flip);
}

unsigned int Resource_Manager::loadTextureFromFile(const GLchar * path, int flip) {

	unsigned int textureID;
	glGenTextures(1, &textureID);
	int width, height, nrComponents;
	stbi_set_flip_vertically_on_load(flip);
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0); //Load image

	if (data) {
		//Loaded image

		//Check image format
		GLenum format;
		if (nrComponents == 1) {

			format = GL_RED;
		}
		else if (nrComponents == 3) {

			format = GL_RGB;
		}
		else if (nrComponents == 4) {

			format = GL_RGBA;
		}

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		//glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //GL_CLAMP_TO_EDGE
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // GL_NEAREST_MIPMAP_NEAREST
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		stbi_image_free(data);
	}
	else {
		//Didn't load image
		std::cout << "Texture failed to load at path: " << path << "\n";
		stbi_image_free(data);
	}

	return textureID;
}

void Resource_Manager::loadCubemap(std::vector<std::string> faces) {

	textures["cubemap"] = loadCubemapFromFile(faces);
}

unsigned int Resource_Manager::loadCubemapFromFile(std::vector<std::string> faces) {

	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;

	stbi_set_flip_vertically_on_load(0);

	for (unsigned int i = 0; i < faces.size(); i++) {

		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data) {

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else {
			std::cout << "Cubemap failed to load at path: " << faces[i] << "\n";
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}