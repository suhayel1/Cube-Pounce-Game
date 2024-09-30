#pragma once

#ifndef COMPONENT_ARRAY_H
#define COMPONENT_ARRAY_H

#include <map>
#include <iostream>
#include <memory>

class IComponentArray
{
public:
	virtual ~IComponentArray() = default;
	virtual void entityDestroyed(int entityID) = 0;
	virtual void display() = 0;
};


template<typename Component>
class ComponentArray : public IComponentArray {
private:
	// key: entity ID, value: component of entity
	std::map<int, Component> componentArray;

public:
	void addComponent(int entityID, Component component);
	void removeComponent(int entityID);
	Component& getComponent(int entityID);
	void entityDestroyed(int entityID) override;
	void display() override;
};

#include "ComponentArray.cpp"

#endif