#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <math.h>
#include <vector>
#include <iostream>

enum Camera_Movement {

	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 20.0f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 90.0f;

class Camera {

public:
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Frontmove;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	float Yaw;
	float Pitch;

	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	//Create Camera
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), Frontmove(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {

		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), Frontmove(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {

		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	glm::mat4 GetViewMatrix() {

		return glm::lookAt(Position, Position + Front, Up);
	}

	void SetPosition(glm::vec3 pos) {

		Position = pos;
	}

	//Move Around
	void ProcessKeyboard(Camera_Movement direction, float deltaTime) {

		//Speed Independent Of Frame Rate
		float velocity = MovementSpeed * deltaTime;

		//Move
		if (direction == FORWARD) {

			Position += Front * velocity;
		}
		if (direction == BACKWARD) {

			Position -= Front * velocity;
		}
		if (direction == LEFT) {

			Position -= Right * velocity;
		}
		if (direction == RIGHT) {

			Position += Right * velocity;
		}
	}

	//Look Around
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true) {

		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		Yaw = std::fmod((Yaw + xoffset), (GLfloat)360.0f);
		Pitch += yoffset;

		if (constrainPitch) {

			if (Pitch > 89.0f) {

				Pitch = 89.0f;
			}
			if (Pitch < -89.0f) {

				Pitch = -89.0f;
			}
		}

		updateCameraVectors();
	}

	//Zoom In And Out
	void ProcessMouseScroll(float yoffset) {

		if (Zoom >= 1.0f && Zoom <= 90.0f) {

			Zoom -= yoffset;
		}
		if (Zoom <= 1.0f) {

			Zoom = 1.0f;
		}
		if (Zoom >= 90.0f) {

			Zoom = 90.0f;
		}
	}

private:
	void updateCameraVectors() {
		//New Direction Which The Player Is Facing
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);

		front.x = cos(glm::radians(Yaw));
		front.y = 1;
		front.z = sin(glm::radians(Yaw));
		Frontmove = glm::normalize(front);

		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}
};

#endif CAMERA_H