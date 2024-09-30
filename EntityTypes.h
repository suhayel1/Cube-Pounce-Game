#pragma once

#include <glm/glm.hpp>
#include <random>

#include "Coordinator.h"
#include "ComponentTypes.h"

void initializePlayer(int& player);
void initializeEnemy(int& enemy);
void initializeCamera(int& camera);
void initializeMouse(int& mouse);
void initializeBoundaries(int (&boundaries)[5]);
void initializeDamagePowerUp(int& dmgPowerUp);