#pragma once

namespace HBL {

	class Scene {
	public:
		Scene(Entity::BaseEntity& player) : _player(player) {  }

		virtual void Enroll_Entities() = 0;
		virtual void Add_Components() = 0;
		virtual void Init_Components() = 0;

		void Set_Level(const std::string& level) { _level = level; }
		std::string& Get_Level() { return _level; }

		const Entity::BaseEntity Get_Player() { return _player; }
	private:
		std::string _level;
		Entity::BaseEntity& _player;
	};

}
