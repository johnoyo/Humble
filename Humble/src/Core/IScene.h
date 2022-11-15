#pragma once
#include "HumbleAPI.h"

namespace HBL {

	class HBL_API IScene {
	public:
		IScene(IEntity& player) : m_Player(player) {  }

		virtual void EnrollEntities() = 0;
		virtual void AddComponents() = 0;
		virtual void InitComponents() = 0;

		IEntity& GetPlayer() { return m_Player; }
	private:
		IEntity& m_Player;
	};

}
