#include "EntityTypes.h"
//#include "ShapeVertices.h"
#include "TextureFiles.h"

extern Coordinator coordinator;

// PLAYER ENTITY
void initializePlayer(int& player) {
    player = coordinator.createEntity();

    coordinator.addComponent<Damage>(player, { 20 });
    coordinator.addComponent<Gravitational>(player, { glm::vec3(0.0f, -9.8f, 0.0f) });
    coordinator.addComponent<Health>(player, { 100 });
    coordinator.addComponent<KeyInput>(player, { -1, -1, -1, -1, -1 });
    coordinator.addComponent<RigidBody>(player, { glm::vec3(0.0f, 0.0f, 0.0f),  glm::vec3(0.0f, 0.0f, 0.0f) });
    coordinator.addComponent<Score>(player, { 0 });
    coordinator.addComponent<Shape>(player, { glm::vec3(0.5f, 0.5f, 0.5f), NULL});
    coordinator.addComponent<Texture>(player, { &playerTexture, 0, 0, 0 });
    coordinator.addComponent<Transformation>(player, { glm::vec3(1.0f, 1.0f, 1.0f),  glm::vec3(0.0f, 0.0f, 0.0f),  glm::vec3(0.0f, 20.0f, 0.0f) });
}

// ENEMY ENTITY
void initializeEnemy(int& enemy) {
    enemy = coordinator.createEntity();

    coordinator.addComponent<Enemy>(enemy, {false});
    coordinator.addComponent<Damage>(enemy, { 20 });
    coordinator.addComponent<Gravitational>(enemy, { glm::vec3(0.0f, -9.8f, 0.0f) });
    coordinator.addComponent<Health>(enemy, { 100 });
    coordinator.addComponent<RigidBody>(enemy, { glm::vec3(0.0f, 0.0f, 0.0f),  glm::vec3(0.0f, 0.0f, 0.0f) });
    coordinator.addComponent<Shape>(enemy, { glm::vec3(0.5f, 0.5f, 0.5f), NULL });
    coordinator.addComponent<Texture>(enemy, { &enemyTexture, 0, 0, 0 });

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<float> randomPosition(-19.0f, 19.0f);

    coordinator.addComponent<Transformation>(enemy, { glm::vec3(1.0f, 1.0f, 1.0f),  glm::vec3(0.0f, 0.0f, 0.0f),  glm::vec3(randomPosition(mt), 20.0f, randomPosition(mt))});
}

// CAMERA ENTITY
void initializeCamera(int& camera) {
    camera = coordinator.createEntity();

    coordinator.addComponent(
        camera,
        Camera{
            glm::vec3(0.0f, 0.0f, 3.0f),    // position
            glm::vec3(0.0f, 0.0f, -1.0f),   // front
            glm::vec3(0.0f, 1.0f, 0.0f),    // up
            glm::vec3(0.0f, 0.0f, 0.0f),    // right
            glm::vec3(0.0f, 1.0f, 0.0f),    // worldUp
            2.5f,                           // movementSpeed
            -90.0f,                         // yaw
            0.0f,                           // pitch
            45.0f,                          // zoom
            true,                           // constrainPitch
            false                           // freeRoamIsEnabled
        }
    );
}

// MOUSE ENTITY
void initializeMouse(int& mouse) {
    mouse = coordinator.createEntity();

    coordinator.addComponent(
        mouse,
        MouseInput{
            0.1f,           // sensitivity
            1080 / 2.0f,     // lastX
            720 / 2.0f,     // lastY
            0.0f,           // xOffset
            0.0f,           // yOffset
            true,           // firstMouse
        }
     );
}

// BOUNDARY ENTITY
void initializeBoundaries(int (&boundaries)[5]) {
    for (int i = 0; i < 5; i++) {
        boundaries[i] = coordinator.createEntity();
        coordinator.addComponent<Shape>(boundaries[i], {glm::vec3(0.5f, 0.5f, 0.5f), NULL});
        coordinator.addComponent<Texture>(boundaries[i], {&boundaryTexture, 0, 0, 0});
    }

    // ground
    coordinator.addComponent<Transformation>(boundaries[0], {glm::vec3(40.0f, 1.0f, 40.0f),  glm::vec3(0.0f, 0.0f, 0.0f),  glm::vec3(0.0f, -1.0f, 0.0f)});

    // right wall
    coordinator.addComponent<Transformation>(boundaries[1], { glm::vec3(1.0f, 2.0f, 40.0f),  glm::vec3(0.0f, 0.0f, 0.0f),  glm::vec3(19.5f, 0.5f, 0.0f) });

    // left wall
    coordinator.addComponent<Transformation>(boundaries[2], { glm::vec3(1.0f, 2.0f, 40.0f),  glm::vec3(0.0f, 0.0f, 0.0f),  glm::vec3(-19.5f, 0.5f, 0.0f) });

    // front wall
    coordinator.addComponent<Transformation>(boundaries[3], { glm::vec3(38.0f, 2.0f, 1.0f),  glm::vec3(0.0f, 0.0f, 0.0f),  glm::vec3(0.0f, 0.5f, -19.5f) });

    // back wall
    coordinator.addComponent<Transformation>(boundaries[4], { glm::vec3(38.0f, 2.0f, 1.0f),  glm::vec3(0.0f, 0.0f, 0.0f),  glm::vec3(0.0f, 0.5f, 19.5f) });
}

// DAMAGE POWER-UP ENTITY
void initializeDamagePowerUp(int& dmgPowerUp) {
    dmgPowerUp = coordinator.createEntity();

    coordinator.addComponent<DamagePowerUp>(dmgPowerUp, {});
    coordinator.addComponent<Shape>(dmgPowerUp, { glm::vec3(0.5f, 0.5f, 0.5f), NULL });
    coordinator.addComponent<Texture>(dmgPowerUp, { &dmgPowerUpTexture, 0, 0, 0 });
    
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<float> randomPosition(-19.0f, 19.0f);

    coordinator.addComponent<Transformation>(dmgPowerUp, { glm::vec3(0.5f, 0.5f, 0.5f),  glm::vec3(0.0f, 0.0f, 0.0f),  glm::vec3(randomPosition(mt), 1.0f, randomPosition(mt)) });
}