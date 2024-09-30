#include "ComponentManager.h"

ComponentManager::ComponentManager() {
	this->nextID = 0;
}

void ComponentManager::entityDestroyed(int entityID) {
	for (auto const& componentArray : this->componentArrays) componentArray->entityDestroyed(entityID);
}

void ComponentManager::display() {
	std::cout << "COMPONENT MANAGER" << std::endl;
	std::cout << "=================" << std::endl;
	std::cout << "Next ID = " << this->nextID << "\n" << std::endl;

	std::cout << "Component Types = {" << std::endl;
	for (auto const& compTypePair : this->componentTypes) {
		const char* componentTypeName = compTypePair.first;
		int componentID = compTypePair.second;

		std::cout << "  " << std::setw(25) << std::left << componentTypeName << std::setw(5) << std::left << "->" << componentID << std::endl;
	}
	std::cout << "}\n" << std::endl;

	std::cout << "Component Arrays = {" << std::endl;
	for (int i = 0; i < this->componentArrays.size(); i++) {
		std::cout << "  " << i << "   ->   ";
		this->componentArrays[i]->display();
	}
	std::cout << "}\n\n" << std::endl;
}

int ComponentManager::getNumComponents() {
	return this->nextID;
}