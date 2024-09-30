#include "ComponentArray.h"

#pragma once

#ifndef COMPONENT_ARRAY_CPP
#define COMPONENT_ARRAY_CPP

template<typename Component>
void ComponentArray<Component>::addComponent(int entityID, Component component) {
	this->componentArray[entityID] = component;
}

template<typename Component>
void ComponentArray<Component>::removeComponent(int entityID) {
	this->componentArray.erase(entityID);
}

template<typename Component>
Component& ComponentArray<Component>::getComponent(int entityID) {
	return this->componentArray[entityID];
}

template<typename Component>
void ComponentArray<Component>::entityDestroyed(int entityID) {
	this->removeComponent(entityID);
}

template<typename Component>
void ComponentArray<Component>::display() {
	std::cout << "[ ";
	for (auto iter = this->componentArray.begin(); iter != this->componentArray.end(); iter++) {
		std::cout << iter->first << " ";
	}
	std::cout << "]" << std::endl;
}

#endif