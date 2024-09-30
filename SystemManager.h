#pragma once

#ifndef SYSTEM_MANAGER_H
#define SYSTEM_MANAGER_H

#include <unordered_map>
#include <vector>
#include <memory>
#include <set>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "BaseSystem.h"

class SystemManager {
private:
	int nextID;

	// key: system name, value: system ID
	std::unordered_map<const char*, int> systemTypes;

	// index: system ID, value: component but set (as bool vector)
	std::vector<std::vector<bool>> systemComponentSets;

	// index: system ID, value: pointer to system object
	std::vector<std::shared_ptr<BaseSystem>> systems;

	bool componentSetsAreEqual(std::vector<bool> entityComponentSet, std::vector<bool> systemComponentSet);

public:
	SystemManager();

	template<typename System>
	std::shared_ptr<System> registerSystem() {
		const char* typeName = typeid(System).name();
		auto system = std::make_shared<System>();

		this->systemTypes[typeName] = this->nextID++;

		this->systemComponentSets.push_back(std::vector<bool>());
		this->systems.push_back(system);

		return system;
	}

	template<typename System>
	int getSystemID() {
		const char* typeName = typeid(System).name();

		return this->systemTypes[typeName];
	}

	template<typename System>
	void updateComponentSet(int componentID, bool value) {
		const char* typeName = typeid(System).name();
		int systemID = this->systemTypes[typeName];

		this->systemComponentSets[systemID][componentID] = value;
	}

	template<typename System>
	void initializeComponentSet(int numComponents) {
		int systemID = this->getSystemID<System>();
		
		for (int i = 0; i < numComponents; i++) {
			this->systemComponentSets[systemID].push_back(false);
		}
	}

	template<typename System>
	std::shared_ptr<System> getSystem() {
		return std::static_pointer_cast<System>(this->systems[this->getSystemID<System>()]);
	}

	void updateAllComponentSets();											// called when registering new component type

	void entityDestroyed(int entityID);

	void updateSystemEntitySets(int entityID, std::vector<bool> entityComponentSet);

	void display();		// for debugging
};

#endif