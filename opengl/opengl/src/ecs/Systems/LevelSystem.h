#pragma once
#include "../Header.h"
#include "RenderingSystem.h"
#include "CollisionSystem.h"
#include "GravitySystem.h"
#include "ScriptingSystem.h"
#include <string>

class LevelSystem {
public:
	void Start();
	void Run();
	void Clear();

	void LoadLevel(const std::string& level_path, ScriptingSystem& scr, GravitySystem& grav, RenderingSystem& rend, VertexBuffer& vertex_buffer, IndexBuffer& index_buffer, Entity::BaseEntity background);
	inline uint32_t GetCurrentLevel() { return current_level; }
private:
	uint32_t current_level = 0;
};