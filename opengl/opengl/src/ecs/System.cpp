#include "System.h"

System::System() { }

void System::EnrollEntity(Entity::BaseEntity& Entity, std::vector<Entity::BaseEntity>& entities) {
	Entity.ID = currentID++;
	entities.push_back(Entity);
}

