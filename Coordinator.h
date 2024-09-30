#pragma once

#ifndef COORDINATOR_H
#define COORDINATOR_H

#include <memory>
#include <vector>
#include <set>
#include <iostream>

#include "EntityManager.h"
#include "ComponentManager.h"
#include "SystemManager.h"

class Coordinator {
private:
	std::unique_ptr<EntityManager> entityManager;
	std::unique_ptr<ComponentManager> componentManager;
	std::unique_ptr<SystemManager> systemManager;

public:
	Coordinator();


	int createEntity();
	void destroyEntity(int entityID);


	template<typename Component>
	void registerComponent() {
		this->componentManager->registerComponent<Component>();

		this->entityManager->updateAllComponentSets();
		this->systemManager->updateAllComponentSets();
	}

	template<typename Component>
	void addComponent(int entityID, Component component) {
		this->componentManager->addComponent<Component>(entityID, component);
		this->entityManager->updateComponentSet(entityID, this->componentManager->getComponentID<Component>(), true);
		this->systemManager->updateSystemEntitySets(entityID, this->entityManager->getComponentSet(entityID));
	}

	template<typename Component>
	void removeComponent(int entityID) {
		this->componentManager->removeComponent(entityID);
		this->entityManager->updateComponentSet(entityID, this->componentManager->getComponentID<Component>(), false);
		this->systemManager->updateSystemEntitySets(entityID, this->entityManager->getComponentSet(entityID));
	}

	template<typename Component>
	Component& getComponent(int entityID) {
		return this->componentManager->getComponent<Component>(entityID);
	}

	template<typename Component>
	int getComponentID() {
		return this->componentManager->getComponentID<Component>();
	}


	template<typename System>
	std::shared_ptr<System> registerSystem() {
		std::shared_ptr<System> newSystem = this->systemManager->registerSystem<System>();
		this->systemManager->initializeComponentSet<System>(this->componentManager->getNumComponents());
		return newSystem;
	}

	template<typename System>
	void updateSystemComponentSet(int componentID, bool value) {
		this->systemManager->updateComponentSet<System>(componentID, value);
	}

	template<typename System>
	int getSystemID() {
		return this->systemManager->getSystemID<System>();
	}

	template <typename System>
	std::shared_ptr<System> getSystem() {
		return this->systemManager->getSystem<System>();
	}

	void display();
};

#endif