#include "EntityManager.h"

EntityManager::EntityManager() {
	this->nextID = 0;
}

int EntityManager::createEntity() {
	int newID = (!this->unusedEntityIDs.empty()) ? getNextUnusedID() : this->nextID++;

	this->entityComponentSets[newID] = std::vector<bool>();

	return newID;
}

int EntityManager::getNextUnusedID() {
	int unusedID = this->unusedEntityIDs.front();

	this->unusedEntityIDs.pop();

	return unusedID;
}

void EntityManager::destroyEntity(int id) {
	if (this->entityComponentSets.find(id) != this->entityComponentSets.end()) {
		this->unusedEntityIDs.push(id);
		this->entityComponentSets.erase(id);
	}
}

void EntityManager::updateAllComponentSets() {
	for (auto& entCompPair : this->entityComponentSets) entCompPair.second.push_back(false);
}

void EntityManager::updateComponentSet(int entityID, int componentID, bool value) {
	this->entityComponentSets[entityID][componentID] = value;
}

std::vector<bool> EntityManager::getComponentSet(int id) {
	return this->entityComponentSets[id];
}

std::set<int> EntityManager::getEntityIDs() {
	std::set<int> entityIDs;

	for (auto const& entCompPair : this->entityComponentSets) entityIDs.insert(entCompPair.first);

	return entityIDs;
}

void EntityManager::display() {
	std::cout << "ENTITY MANAGER" << std::endl;
	std::cout << "==============" << std::endl;
	std::cout << "Next ID = " << this->nextID << "\n" << std::endl;
	std::cout << "Unused Entity IDs = [ ";

	std::queue<int> unusedEntityIDsCopy = this->unusedEntityIDs;
	while (!unusedEntityIDsCopy.empty()) {
		std::cout << unusedEntityIDsCopy.front();
		unusedEntityIDsCopy.pop();
		if (!unusedEntityIDsCopy.empty()) std::cout << ", ";
	}
	std::cout << " ]\n" << std::endl;

	std::cout << "Entity Component Sets = {" << std::endl;
	for (auto iter = this->entityComponentSets.begin(); iter != this->entityComponentSets.end(); iter++) {
		std::cout << "  " << iter->first << "   ->   ";
		for (bool val : iter->second) std::cout << int(val) << " ";
		std::cout << std::endl;
	}
	std::cout << "}\n\n" << std::endl;
}

void EntityManager::initializeComponentSet(int id, int numComponents) {
	for (int i = 0; i < numComponents; i++) this->entityComponentSets[id].push_back(false);
}