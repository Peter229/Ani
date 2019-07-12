#pragma once
#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "stb_image.h"
#include "glad\glad.h"
#include <glm/glm.hpp>

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <strstream>

class Resource_Manager {
public:

	static unsigned int getTexture(std::string name);

	static void loadTexture(const GLchar *file, std::string name, int flip);
	static void loadCubemap(std::vector<std::string> faces);

	static void clearData();

private:

	static unsigned int loadTextureFromFile(const GLchar * path, int flip);
	static unsigned int loadCubemapFromFile(std::vector<std::string> faces);
};

#endif