#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include "glm\glm.hpp"
#include "glm\gtx\quaternion.hpp"
#include "glm\gtx\compatibility.hpp"
#include "glm\gtx\transform.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h"
#include "resource_manager.h"
#include <glad/glad.h>
#include <iostream>
#include <experimental/filesystem>
#include <fstream>
#include <vector>
#include <string>

void RemoveQuotes(std::string& str);

int GetFileLength(std::istream& file);

void IgnoreLine(std::istream& file, int length);

void ComputeQuatW(glm::quat& quat);