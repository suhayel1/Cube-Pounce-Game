#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <array>
#include <string>
#include <unordered_map>
#include <stb_image.h>

#include "BaseSystem.h"
#include "Coordinator.h"
#include "ComponentTypes.h"
#include "Shader.h"
#include "WindowManager.h"

class KeyInputSystem : public BaseSystem {
public:
	void initialize();
	void update();
};

class MouseInputSystem : public BaseSystem {
public:
	void initialize();
	void update();
};

class BoundaryCollisionSystem : public BaseSystem {
private:
	int* boundaries;

public:
	void initialize();
	void setBounds(int* boundaries);
	void update();
};


class CameraSystem : public BaseSystem {
private:
	std::shared_ptr<MouseInputSystem> mouseInputSystem;

public:
	void initialize();
	void update();
};

class DamagePowerUpSystem : public BaseSystem {
private:
	int player;
public:
	void initialize();
	void setPlayer(int player);
	void update(float time);
};

class EnemyMovementSystem : public BaseSystem {
private:
	int player;

public:
	void initialize();
	void setPlayer(int player);
	void update();
};


class EnemyCollisionSystem : public BaseSystem {
public:
	void initialize();
	void update();
};


class PlayerSystem : public BaseSystem {
private:
	std::shared_ptr<MouseInputSystem> mouseInputSystem;
	std::shared_ptr<CameraSystem> cameraSystem;

public:
	void initialize();
	void update();
};


class PlayerCameraSystem : public BaseSystem {
private:
	std::shared_ptr<PlayerSystem> playerSystem;
	std::shared_ptr<CameraSystem> cameraSystem;

public:
	void initialize();
	void update();
};


class PlayerCollisionSystem : public BaseSystem {
private:
	int player;

public:
	void initialize();
	void setPlayer(int player);
	void update();
};


class PhysicsSystem : public BaseSystem {
public:
	void initialize();
	void update(float timeStep);
};

class RenderSystem : public BaseSystem {
private:
	std::shared_ptr<CameraSystem> cameraSystem;

	Shader playerShader;
	Shader enemyShader;
	Shader boundaryShader;
	Shader dmgPowerUpShader;

	unsigned int cubeVBO, cubeVAO;

	std::unordered_map<std::string, unsigned int> textures;
	std::unordered_map<unsigned int, std::array<int, 3>> textureParams;	// width, height, nrChannels

public:
	void initialize();
	void setShaders();
	void setRenderData();
	void setTextureData(std::string texture);
	void setVertexData(unsigned int& vao, unsigned int& vbo, std::vector<float>& vertices);
	glm::mat4 getFullTranformation(int entity);
	void update();
	void shutDown();
};