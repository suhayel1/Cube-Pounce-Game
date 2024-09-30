#pragma once

#ifndef COMPONENT_MANAGER_H
#define COMPONENT_MANAGER_H

#include <unordered_map>
#include <memory>
#include <vector>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "ComponentArray.h"

class ComponentManager {
private:
	int nextID;

	// maps from component name to component id
	std::unordered_map<const char*, int> componentTypes;

	// index: component type ID, value: shared ptr to component array
	std::vector<std::shared_ptr<IComponentArray>> componentArrays;

	template<typename Component>
	std::shared_ptr<ComponentArray<Component>> getComponentArray() {
		return std::static_pointer_cast<ComponentArray<Component>>(this->componentArrays[this->getComponentID<Component>()]);
	}

	template<typename Component>
	const char* getComponentTypename() {
		return typeid(Component).name();
	}

public:
	ComponentManager();

	template<typename Component>
	void registerComponent() {
		const char* typeName = typeid(Component).name();

		this->componentTypes[typeName] = this->nextID++;
		this->componentArrays.push_back(std::make_shared<ComponentArray<Component>>());
	}

	template<typename Component>
	int getComponentID() {
		const char* typeName = typeid(Component).name();

		return this->componentTypes[typeName];
	}

	template<typename Component>
	void addComponent(int entityID, Component component) {
		this->getComponentArray<Component>()->addComponent(entityID, component);
	}

	template<typename Component>
	void removeComponent(int entityID) {
		this->getComponentArray<Component>()->removeComponent(entityID);
	}

	template<typename Component>
	Component& getComponent(int entityID) {
		return this->getComponentArray<Component>()->getComponent(entityID);
	}

	void entityDestroyed(int entityID);

	void display();		// for debugging

	int getNumComponents();
};

#endif