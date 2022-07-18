#pragma once
#include "HumbleAPI.h"

namespace HBL {

	class HBL_API IScene {
	public:
		IScene(IEntity& player) : _player(player) {  }

		virtual void Enroll_Entities() = 0;
		virtual void Add_Components() = 0;
		virtual void Init_Components() = 0;
		virtual void Init_Systems() = 0;

		IEntity& Get_Player() { return _player; }
	private:
		std::string _level;
		IEntity& _player;
	};

}
