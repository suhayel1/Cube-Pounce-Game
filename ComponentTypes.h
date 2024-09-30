#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "Shader.h"

struct Empty {};

struct Enemy {
	bool isDead;
};

struct Camera {
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;
	float movementSpeed;
	//float mouseSensitivity;
	float yaw;
	float pitch;
	float zoom;
	bool constrainPitch;
	bool freeRoamIsEnabled;
};

struct Damage {
	int points;
};

struct DamagePowerUp {};

struct Gravitational {
	glm::vec3 force;
};

struct Health {
	int points;
};

struct KeyInput {
	int w;
	int a;
	int s;
	int d;
	int space;
};

struct MouseInput {
	float sensitivity;
	float lastX;
	float lastY;
	float xOffset;
	float yOffset;
	bool firstMouse;
};

struct Node {
	bool isValid;
	float x;
	float z;
};

struct RigidBody {
	glm::vec3 velocity;
	glm::vec3 acceleration;
};

struct Score {
	int points;
};

struct Shape {
	glm::vec3 color;
	Shader* shader;
};

struct Texture {
	std::string* fileName;
	int width;
	int height;
	int nrChannels;
};

struct Transformation {
	glm::vec3 scale;
	glm::vec3 rotation;
	glm::vec3 translation;
};