#pragma once
#include "HumbleAPI.h"

namespace HBL {

	class HBL_API IScene {
	public:
		IScene(IEntity& player) : m_Player(player) {  }

		virtual void Enroll_Entities() = 0;
		virtual void Add_Components() = 0;
		virtual void Init_Components() = 0;
		virtual void Init_Systems() = 0;

		IEntity& Get_Player() { return m_Player; }
	private:
		std::string m_Level;
		IEntity& m_Player;
	};

}
