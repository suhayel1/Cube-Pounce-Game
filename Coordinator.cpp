#include "Coordinator.h"

Coordinator::Coordinator() {
	this->entityManager = std::make_unique<EntityManager>();
	this->componentManager = std::make_unique<ComponentManager>();
	this->systemManager = std::make_unique<SystemManager>();
}


int Coordinator::createEntity() {
	int newEntityID = this->entityManager->createEntity();
	this->entityManager->initializeComponentSet(newEntityID, this->componentManager->getNumComponents());
	return newEntityID;
}

void Coordinator::destroyEntity(int entityID) {
	this->entityManager->destroyEntity(entityID);
	this->componentManager->entityDestroyed(entityID);
	this->systemManager->entityDestroyed(entityID);
}

void Coordinator::display() {
	this->entityManager->display();
	this->componentManager->display();
	this->systemManager->display();
	std::cout << "==================================================\n\n" << std::endl;
}