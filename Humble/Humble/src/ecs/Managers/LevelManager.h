#pragma once
#include "../Header.h"
#include "../Systems/RenderingSystem.h"
#include "../Systems/CollisionSystem.h"
#include "../Systems/GravitySystem.h"
#include "../Systems/ScriptingSystem.h"
#include <string>

namespace HBL {

	class LevelManager {
	public:
		LevelManager(const LevelManager&) = delete;

		static LevelManager& Get() {
			static LevelManager instance;
			return instance;
		}

		static void Load_Level(const std::string& level_path) {
			Get().m_current_level++;
			Get().m_level_path = level_path;
		}

		static void Load_Level(const std::string& level_path, ScriptingSystem& scr, GravitySystem& grav, RenderingSystem& rend, VertexBuffer& vertex_buffer, IndexBuffer& index_buffer, Entity::BaseEntity background, bool first) {
			return Get().ILoadLevel(level_path, scr, grav, rend, vertex_buffer, index_buffer, background, first);
		}

		inline static uint32_t GetCurrentLevel() { return Get().IGetCurrentLevel(); }
		inline static const std::string& GetLevelPath() { return Get().IGetCurrentPath(); }

	private:
		LevelManager() {}

		void ILoadLevel(const std::string& level_path, ScriptingSystem& scr, GravitySystem& grav, RenderingSystem& rend, VertexBuffer& vertex_buffer, IndexBuffer& index_buffer, Entity::BaseEntity background, bool first);
		inline uint32_t IGetCurrentLevel() { return m_current_level; }
		inline const std::string& IGetCurrentPath() { return m_level_path; }

		std::string m_level_path;
		uint32_t m_current_level = -1;
	};

}
