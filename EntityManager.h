#pragma once

#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <queue>
#include <map>
#include <vector>
#include <set>
#include <iostream>

class EntityManager {
private:
	// holds ID of next entity to be created. also represents total number of entities created and destroyed.
	int nextID;

	// when entity gets destroyed, its ID gets stored here so that when new entity is created, it will get it the from here. nextID isn't updated then.
	std::queue<int> unusedEntityIDs;

	// holds all entities that haven't been destroyed, where key is entity ID, and value is component set represented as bits (bool value).
	std::map<int, std::vector<bool>> entityComponentSets;

	// gets called by createEntity() if unusedEntityIDs isn't empty
	int getNextUnusedID();

public:
	EntityManager();

	int createEntity();
	void destroyEntity(int id);
	void updateAllComponentSets();											// called when registering new component type
	void updateComponentSet(int entityID, int componentID, bool value);		// called when adding new component to entity
	std::vector<bool> getComponentSet(int id);
	std::set<int> getEntityIDs();
	void display();															// for debugging
	void initializeComponentSet(int id, int numComponents);					// called when new entity is created after components have been registered
};

#endif