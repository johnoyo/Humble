#pragma once
#include "../Header.h"
#include "../Systems/RenderingSystem.h"
#include "../Systems/CollisionSystem.h"
#include "../Systems/GravitySystem.h"
#include "../Systems/ScriptingSystem.h"
#include <string>

class LevelManager {
public:
	LevelManager(const LevelManager&) = delete;

	static LevelManager& Get() {
		static LevelManager instance;
		return instance;
	}

	static void Load_Level(const std::string& level_path, ScriptingSystem& scr, GravitySystem& grav, RenderingSystem& rend, VertexBuffer& vertex_buffer, IndexBuffer& index_buffer, Entity::BaseEntity background) {
		return Get().ILoadLevel(level_path, scr, grav, rend, vertex_buffer, index_buffer, background);
	}

	inline static uint32_t GetCurrentLevel() { return Get().IGetCurrentLevel(); }
private:
	LevelManager() {}

	void ILoadLevel(const std::string& level_path, ScriptingSystem& scr, GravitySystem& grav, RenderingSystem& rend, VertexBuffer& vertex_buffer, IndexBuffer& index_buffer, Entity::BaseEntity background);
	inline uint32_t IGetCurrentLevel() { return m_current_level; }
	uint32_t m_current_level = 0;
};