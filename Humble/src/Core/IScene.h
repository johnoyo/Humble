#pragma once
#include "HumbleAPI.h"

namespace HBL {

	class HBL_API IScene {
	public:
		IScene(IEntity& player, IEntity& camera) : m_Player(player), m_Camera(camera) {  }

		virtual void Enroll_Entities() = 0;
		virtual void Add_Components() = 0;
		virtual void Init_Components() = 0;
		virtual void Init_Systems() = 0;

		IEntity& GetPlayer() { return m_Player; }
		IEntity& GetCamera() { return m_Camera; }
	private:
		IEntity& m_Player;
		IEntity& m_Camera;
	};

}
