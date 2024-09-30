#include "SystemManager.h"

SystemManager::SystemManager() {
	this->nextID = 0;
}

void SystemManager::updateAllComponentSets() {
	for (auto& componentSet : this->systemComponentSets) componentSet.push_back(false);
}

void SystemManager::entityDestroyed(int entityID) {
	for (auto const& entitySet : this->systems) entitySet->entities.erase(entityID);
}

void SystemManager::updateSystemEntitySets(int entityID, std::vector<bool> entityComponentSet) {
	for (int i = 0; i < this->systemTypes.size(); i++) {
		if (this->componentSetsAreEqual(entityComponentSet, this->systemComponentSets[i])) this->systems[i]->entities.insert(entityID);
		else this->systems[i]->entities.erase(entityID);
	}
}

bool SystemManager::componentSetsAreEqual(std::vector<bool> entityComponentSet, std::vector<bool> systemComponentSet) {
	for (int i = 0; i < systemComponentSet.size(); i++) {
		if (systemComponentSet[i] && !entityComponentSet[i]) return false;
	}

	return true;
}

void SystemManager::display() {
	std::cout << "SYSTEM MANAGER" << std::endl;
	std::cout << "==============" << std::endl;
	std::cout << "Next ID = " << this->nextID << "\n" << std::endl;

	std::cout << "System Types = {" << std::endl;
	for (auto const& sysTypePair : this->systemTypes) {
		const char* systemTypeName = sysTypePair.first;
		int systemID = sysTypePair.second;

		std::cout << "  " << std::setw(25) << std::left << systemTypeName << std::setw(5) << std::left << "->" << systemID << std::endl;
	}
	std::cout << "}\n" << std::endl;

	std::cout << "System Component Sets = {" << std::endl;
	for (int i = 0; i < this->systemComponentSets.size(); i++) {
		std::cout << "  " << i << "   ->   ";
		for (bool val : this->systemComponentSets[i]) std::cout << int(val) << " ";
		std::cout << std::endl;
	}
	std::cout << "}\n" << std::endl;

	std::cout << "System Entity Sets = {" << std::endl;
	for (int i = 0; i < this->systems.size(); i++) {
		std::cout << "  " << i << "   ->   [ ";
		for (auto iter = this->systems[i]->entities.begin(); iter != this->systems[i]->entities.end(); iter++) {
			std::cout << *iter << " ";
		}
		std::cout << "]" << std::endl;
	}
	std::cout << "}\n\n" << std::endl;
}