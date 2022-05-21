#pragma once
#include <iostream>
#include <vector>
#include "Entities.h"
#include "Components.h"

class System {
public:

	System();

	void EnrollEntity(Entity::BaseEntity &Entity, std::vector<Entity::BaseEntity>& entities);

	template<typename T>
	void AddComponent(int &ComponentEntityID, int& EntityID, std::vector<T>& ComponentVector) {
		T c;
		ComponentVector.push_back(c);
		ComponentEntityID = ComponentVector.size()-1;
		EntityID = ComponentEntityID;
	}

	template<typename T>
	T& GetComponent(int ComponenetEntityID, std::vector<T>& ComponentVector) {
		return ComponentVector.at(ComponenetEntityID);
	}

	template<typename T>
	void PrintComponents(std::vector<T> ComponentVector) { }

	template<>
	void PrintComponents<Component::Transform>(std::vector<Component::Transform> ComponentVector) {
		std::cout << "Positions: \n";
		for (unsigned int i = 0; i < ComponentVector.size(); i++) {
			std::cout << "x,y: " << ComponentVector.at(i).position.x << ", " << ComponentVector.at(i).position.y << "\n";
		}
	}
	template<>
	void PrintComponents<Component::Health>(std::vector<Component::Health> ComponentVector) {
		std::cout << "health: \n";
		for (unsigned int i = 0; i < ComponentVector.size(); i++) {
			std::cout << "h: " << ComponentVector.at(i).health <<  "\n";
		}
	}

private:
	int currentID = 0;
};
