#include <string>
#include <glm/glm.hpp>
#include <iostream>
#include <algorithm>

#include "WindowManager.h"
#include "EntityManager.h"
#include "ComponentManager.h"
#include "SystemManager.h"
#include "Coordinator.h"
#include "EntityTypes.h"
#include "ComponentTypes.h"
#include "SystemTypes.h"

// variables
const unsigned int GLFW_MAJOR_VERS = 3;
const unsigned int GLFW_MINOR_VERS = 3;
const unsigned int SCREEN_WIDTH = 1080;
const unsigned int SCREEN_HEIGHT = 720;

const std::string GAME_NAME = "Cube Pounce";

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

// deals with ECS
Coordinator coordinator;

WindowManager windowManager;

int numPowerUps = 0;

int main() {
    windowManager = WindowManager(GLFW_MAJOR_VERS, GLFW_MINOR_VERS, SCREEN_WIDTH, SCREEN_HEIGHT, GAME_NAME);
    if (!windowManager.isInitialized()) return 1;   // error

    // component registration
    coordinator.registerComponent<Empty>();
    coordinator.registerComponent<Enemy>();
    coordinator.registerComponent<Camera>();
    coordinator.registerComponent<Damage>();
    coordinator.registerComponent<DamagePowerUp>();
    coordinator.registerComponent<Gravitational>();
    coordinator.registerComponent<Health>();
    coordinator.registerComponent<KeyInput>();
    coordinator.registerComponent<MouseInput>();
    coordinator.registerComponent<RigidBody>();
    coordinator.registerComponent<Score>();
    coordinator.registerComponent<Shape>();
    coordinator.registerComponent<Texture>();
    coordinator.registerComponent<Transformation>();

    // system registration/declaration
    auto boundaryCollisionSystem = coordinator.registerSystem<BoundaryCollisionSystem>();
    auto cameraSystem = coordinator.registerSystem<CameraSystem>();
    auto dmgPowerUpSystem = coordinator.registerSystem<DamagePowerUpSystem>();
    auto enemyMovementSystem = coordinator.registerSystem<EnemyMovementSystem>();
    auto enemyCollisionSystem = coordinator.registerSystem<EnemyCollisionSystem>();
    auto keyInputSystem = coordinator.registerSystem<KeyInputSystem>();
    auto mouseInputSystem = coordinator.registerSystem<MouseInputSystem>();
    auto physicsSystem = coordinator.registerSystem<PhysicsSystem>();
    auto playerCameraSystem = coordinator.registerSystem<PlayerCameraSystem>();
    auto playerSystem = coordinator.registerSystem<PlayerSystem>();
    auto playerCollisionSystem = coordinator.registerSystem<PlayerCollisionSystem>();
    auto renderSystem = coordinator.registerSystem<RenderSystem>();

    // system initialization
    boundaryCollisionSystem->initialize();
    cameraSystem->initialize();
    dmgPowerUpSystem->initialize();
    enemyMovementSystem->initialize();
    enemyCollisionSystem->initialize();
    mouseInputSystem->initialize();
    physicsSystem->initialize();
    playerSystem->initialize();
    playerCollisionSystem->initialize();
    keyInputSystem->initialize();
    playerCameraSystem->initialize();
    renderSystem->initialize();

    // entity declaration
    int player;
    int camera;
    int mouse;
    int boundaries[5];
    std::vector<int> enemies;
    int dmgPowerUp;

    // entity initialization
    initializePlayer(player);
    initializeCamera(camera);
    initializeMouse(mouse);
    initializeBoundaries(boundaries);

    boundaryCollisionSystem->setBounds(boundaries);
    enemyMovementSystem->setPlayer(player);
    playerCollisionSystem->setPlayer(player);
    dmgPowerUpSystem->setPlayer(player);

    renderSystem->setShaders();
    renderSystem->setRenderData();

    float enemySpawnTime = 2.0f;

    coordinator.display();

    while (windowManager.isRunning()) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        if (enemies.size() < 10 && currentFrame - enemySpawnTime >= 4.0f) {
            enemies.push_back(-1);
            initializeEnemy(enemies.back());
            enemySpawnTime = currentFrame;
        }

        if (currentFrame >= 8.0f && numPowerUps < 1) {
            initializeDamagePowerUp(dmgPowerUp);
            numPowerUps++;
        }

        windowManager.processInput();

        renderSystem->setShaders();

        mouseInputSystem->update();
        cameraSystem->update();
        playerSystem->update();
        keyInputSystem->update();
        playerCameraSystem->update();
        dmgPowerUpSystem->update(currentFrame);
        enemyMovementSystem->update();
        enemyCollisionSystem->update();
        boundaryCollisionSystem->update();
        playerCollisionSystem->update();
        physicsSystem->update(deltaTime);
        renderSystem->update();
        windowManager.update();


    }

    renderSystem->shutDown();
    windowManager.shutDown();

	return 0;
}