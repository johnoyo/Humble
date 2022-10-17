#pragma once
#include <iostream>
#include <vector>
#include "IEntity.h"
#include "Components.h"
#include "HumbleAPI.h"
#include <type_traits>

namespace HBL {

	class HBL_API System {
	public:

		System();

		void EnrollEntity(IEntity& Entity, std::vector<IEntity>& entities) 
		{
			Entity.ID = currentID++;
			entities.push_back(Entity);
		}

		template<typename T>
		void AddComponent(int& ComponentEntityID, int& EntityID, std::vector<T>& ComponentVector) 
		{
			T c;
			ComponentVector.push_back(c);
			ComponentEntityID = ComponentVector.size() - 1;
			EntityID = ComponentEntityID;
		}

		template<typename T>
		T& GetComponent(int ComponenetEntityID, std::vector<T>& ComponentVector) 
		{
			return ComponentVector[ComponenetEntityID];
		}

		template<typename T>
		void AddComponent(IEntity& Entity, const std::string& Component, std::vector<IEntity>& entities, std::vector<T>& ComponentVector)
		{
			T c;
			ComponentVector.push_back(c);

			Entity.components.emplace(Component, ComponentVector.size() - 1);
			entities.at(Entity.ID).components.emplace(Component, ComponentVector.size() - 1);
		}

		template<typename T>
		T& GetComponent(IEntity& Entity, const std::string& Component, std::vector<T>& ComponentVector)
		{
			return ComponentVector.at(Entity.components[Component]);
		}

		void Flush(std::vector<IEntity>& entities)
		{
			entities.clear();
			currentID = 0;
		}

		template<typename T>
		void PrintComponents(std::vector<T>& ComponentVector) { }

		template<>
		void PrintComponents<Component::Transform>(std::vector<Component::Transform>& ComponentVector) 
		{
			std::cout << "Positions: \n";
			for (unsigned int i = 0; i < ComponentVector.size(); i++) {
				std::cout << "x,y: " << ComponentVector.at(i).position.x << ", " << ComponentVector.at(i).position.y << "\n";
			}
		}

	private:
		int currentID = 0;
	};

}

