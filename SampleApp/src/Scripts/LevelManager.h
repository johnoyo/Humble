#pragma once
#include "../../../Humble/src/Humble.h"

extern HBL::IEntity background;
extern HBL::IEntity player;
extern HBL::IEntity enemy;
extern HBL::IEntity wall[400];
extern HBL::IEntity level[10000];
extern HBL::IEntity camera;
extern HBL::IEntity lvlHandler;
extern HBL::IEntity sps;

#include <string>
#include <vector>
#include <fstream>

namespace HBL {

	class LevelManager {
	public:
		LevelManager(const LevelManager&) = delete;

		static LevelManager& Get() 
		{
			static LevelManager instance;
			return instance;
		}

		static void Load_Level(const std::string& level_path) 
		{
			Get().m_current_level++;
			Get().m_level_path = level_path;
		}

		static void Load_Level(const std::string& level_path, bool first) 
		{
			Get().m_current_level++;
			return Get().ILoadLevel(level_path, GlobalSystems::scriptingSystem, GlobalSystems::gravitySystem, GlobalSystems::spriteRendererSystem, Renderer::Get().GetVertexBuffer(0), Renderer::Get().GetIndexBuffer(0), background, first);
		}

		inline static uint32_t GetCurrentLevel() { return Get().IGetCurrentLevel(); }
		inline static const std::string& GetLevelPath() { return Get().IGetCurrentPath(); }

	private:
		LevelManager() {}

		void ILoadLevel(const std::string& level_path, ScriptingSystem& scr, GravitySystem& grav, SpriteRendererSystem& rend, VertexBuffer& vertex_buffer, IndexBuffer& index_buffer, IEntity& background, bool first);
		inline uint32_t IGetCurrentLevel() { return m_current_level; }
		inline const std::string& IGetCurrentPath() { return m_level_path; }

		std::string m_level_path;
		uint32_t m_current_level = -1;
	};

}
