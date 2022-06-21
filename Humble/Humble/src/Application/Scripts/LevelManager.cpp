#include "LevelManager.h"

namespace HBL {

	void LevelManager::ILoadLevel(const std::string& level_path, ScriptingSystem& scr, GravitySystem& grav, RenderingSystem& rend, VertexBuffer& vertex_buffer, IndexBuffer& index_buffer, IEntity background, bool first)
	{
		std::ifstream is(level_path);

		char c;
		char acc[3];
		int i = 0, j = 0, s = 0, h = 0, w = 0;
		int index = 0;

		// Parse level dimensions
		while (is.get(c)) {
			if (c == '\n') break;

			if (c == ',') {
				index = 0;
				h = atoi(acc);
				continue;
			}
			if (c == '.') {
				index = 0;
				w = atoi(acc);
				continue;
			}
			acc[index] = c;
			index++;
		}

		i = h - 1;

		struct pos {
			int i;
			int j;
			float k;
		};

		// Parse level content
		std::vector<pos> p;
		while (is.get(c)) {
			if (c == 'B') {
				p.push_back({ i,j,6.0f });
			}
			else if (c == 'G') {
				p.push_back({ i,j,1.0f });
			}
			else if (c == 'C') {
				p.push_back({ i,j,3.0f });
			}
			else if (c == 'R') {
				p.push_back({ i,j,15.0f });
			}
			else if (c == 'E') {
				p.push_back({ i,j,4.0f });
			}
			else if (c == '>') {
				p.push_back({ i,j,8.0f });
			}
			else if (c == 'P') {
				p.push_back({ i,j,5.0f });
				s = p.size() - 1;
			}
			if (j == w) {
				j = 0;
				i--;
				continue;
			}
			j++;
		}

		vertex_buffer.Reset();
		int level_index = 0;
		int wall_index = 0;
		int enemy_index = 0;
		int collectible_index = 0;

		// Reset all component properties of entities
		for (uint32_t i = 0; i < entities.size(); i++) {
			IEntity& entt = entities.at(i);
			if (TRY_FIND_COMPONENT(Transform, entt)) {
				GET_COMPONENT(Transform, entt).Static = true;
				GET_COMPONENT(Transform, entt).Enabled = false;
			}
			if (TRY_FIND_COMPONENT(Material, entt)) {
				GET_COMPONENT(Material, entt).texture = "-";
				GET_COMPONENT(Material, entt).subTexture.path = "-";
				GET_COMPONENT(Material, entt).Enabled = false;
			}
			if (TRY_FIND_COMPONENT(Script, entt)) {
				GET_COMPONENT(Script, entt).Enabled = false;
			}
			if (TRY_FIND_COMPONENT(CollisionBox, entt)) {
				GET_COMPONENT(CollisionBox, entt).Enabled = false;
			}
			if (TRY_FIND_COMPONENT(Shadow, entt)) {
				GET_COMPONENT(Shadow, entt).Enabled = false;
			}
		}

		// Set up background
		GET_COMPONENT(Transform, background).position.x = 0.0f;
		GET_COMPONENT(Transform, background).position.y = 0.0f;
		GET_COMPONENT(Transform, background).scale.x = 1920.0f;
		GET_COMPONENT(Transform, background).scale.y = 1080.0f;
		GET_COMPONENT(Transform, background).Static = false;
		GET_COMPONENT(Transform, background).Enabled = true;
		GET_COMPONENT(Material, background).Enabled = true;
		GET_COMPONENT(Material, background).texture = "res/textures/brickWall_2.jpg";

		// Re enable lvlHandler
		GET_COMPONENT(Script, lvlHandler).Enabled = true;

		// Update the position of the entities
		for (uint32_t i = 0; i < p.size(); i++) {
			if (i == s) continue;
			if (p.at(i).k == 1.0f) {
				GET_COMPONENT(Transform, level[level_index]).scale.x = 30.0f;
				GET_COMPONENT(Transform, level[level_index]).scale.y = 30.0f;
				GET_COMPONENT(Transform, level[level_index]).position.x = p.at(i).j * GET_COMPONENT(Transform, level[level_index]).scale.x;
				GET_COMPONENT(Transform, level[level_index]).position.y = p.at(i).i * GET_COMPONENT(Transform, level[level_index]).scale.y;
				GET_COMPONENT(Transform, level[level_index]).Static = true;
				GET_COMPONENT(Transform, level[level_index]).Enabled = true;

				GET_COMPONENT(Material, level[level_index]).Enabled = true;
				GET_COMPONENT(Material, level[level_index]).texture = "res/textures/coinA.png";
				//GET_COMPONENT(CollisionBox, level[level_index]).Enabled = true;

				level_index++;
			}
			else if (p.at(i).k == 6.0f) {
				GET_COMPONENT(Transform, wall[wall_index]).scale.x = 30.0f;
				GET_COMPONENT(Transform, wall[wall_index]).scale.y = 30.0f;
				GET_COMPONENT(Transform, wall[wall_index]).position.x = p.at(i).j * GET_COMPONENT(Transform, wall[wall_index]).scale.x;
				GET_COMPONENT(Transform, wall[wall_index]).position.y = p.at(i).i * GET_COMPONENT(Transform, wall[wall_index]).scale.y;
				GET_COMPONENT(Transform, wall[wall_index]).Static = true;
				GET_COMPONENT(Transform, wall[wall_index]).Enabled = true;

				GET_COMPONENT(CollisionBox, wall[wall_index]).Enabled = true;

				GET_COMPONENT(Material, wall[wall_index]).Enabled = true;
				GET_COMPONENT(Material, wall[wall_index]).texture = "res/textures/brick_3.png";

				if (!first)
					GET_COMPONENT(Shadow, wall[wall_index]).Enabled = true;

				wall_index++;
			}
			else if (p.at(i).k == 4.0f) {
				GET_COMPONENT(Transform, enemy).scale.x = 30.0f;
				GET_COMPONENT(Transform, enemy).scale.y = 30.0f;
				GET_COMPONENT(Transform, enemy).position.x = p.at(i).j * GET_COMPONENT(Transform, enemy).scale.x;
				GET_COMPONENT(Transform, enemy).position.y = p.at(i).i * GET_COMPONENT(Transform, enemy).scale.y;
				GET_COMPONENT(Transform, enemy).Static = false;
				GET_COMPONENT(Transform, enemy).Enabled = true;

				GET_COMPONENT(CollisionBox, enemy).Enabled = true;

				GET_COMPONENT(Script, enemy).Enabled = true;

				GET_COMPONENT(Material, enemy).Enabled = true;
				GET_COMPONENT(Material, enemy).texture = "res/textures/enemy.png";

				GET_COMPONENT(Shadow, enemy).Enabled = true;
				enemy_index++;
			}
		}

		ENGINE_LOG("lvl indx: %d", level_index);
		ENGINE_LOG("wall indx: %d", wall_index);
		ENGINE_LOG("p size: %d", p.size());

		// Upadate the position of the player last
		GET_COMPONENT(Transform, player).scale.x = 29.0f;
		GET_COMPONENT(Transform, player).scale.y = 29.0f;
		GET_COMPONENT(Transform, player).position.x = p.at(s).j * GET_COMPONENT(Transform, player).scale.x;
		GET_COMPONENT(Transform, player).position.y = p.at(s).i * GET_COMPONENT(Transform, player).scale.y;
		GET_COMPONENT(Transform, player).Static = false;
		GET_COMPONENT(Transform, player).Enabled = true;

		GET_COMPONENT(CollisionBox, player).Enabled = true;

		GET_COMPONENT(Script, player).Enabled = true;

		GET_COMPONENT(Material, player).Enabled = true;
		GET_COMPONENT(Material, player).texture = "res/textures/player_r.png";

		// Recalculate all collision boxes
		for (uint32_t i = 0; i < entities.size(); i++) {
			if (TRY_FIND_COMPONENT(CollisionBox, entities.at(i)) && TRY_FIND_COMPONENT(Transform, entities.at(i))) {
				if (GET_COMPONENT(CollisionBox, entities.at(i)).Enabled) {
					GET_COMPONENT(CollisionBox, entities.at(i)).tl.x = GET_COMPONENT(Transform, entities.at(i)).position.x - GET_COMPONENT(Transform, entities.at(i)).scale.x / 2.0f;
					GET_COMPONENT(CollisionBox, entities.at(i)).tl.y = GET_COMPONENT(Transform, entities.at(i)).position.y + GET_COMPONENT(Transform, entities.at(i)).scale.y / 2.0f;
																	   				 
					GET_COMPONENT(CollisionBox, entities.at(i)).tr.x = GET_COMPONENT(Transform, entities.at(i)).position.x + GET_COMPONENT(Transform, entities.at(i)).scale.x / 2.0f;
					GET_COMPONENT(CollisionBox, entities.at(i)).tr.y = GET_COMPONENT(Transform, entities.at(i)).position.y + GET_COMPONENT(Transform, entities.at(i)).scale.y / 2.0f;
																	  
					GET_COMPONENT(CollisionBox, entities.at(i)).br.x = GET_COMPONENT(Transform, entities.at(i)).position.x + GET_COMPONENT(Transform, entities.at(i)).scale.x / 2.0f;
					GET_COMPONENT(CollisionBox, entities.at(i)).br.y = GET_COMPONENT(Transform, entities.at(i)).position.y - GET_COMPONENT(Transform, entities.at(i)).scale.y / 2.0f;
																	   
					GET_COMPONENT(CollisionBox, entities.at(i)).bl.x = GET_COMPONENT(Transform, entities.at(i)).position.x - GET_COMPONENT(Transform, entities.at(i)).scale.x / 2.0f;
					GET_COMPONENT(CollisionBox, entities.at(i)).bl.y = GET_COMPONENT(Transform, entities.at(i)).position.y - GET_COMPONENT(Transform, entities.at(i)).scale.y / 2.0f;
				}
			}
		}

		is.close();

		rend.Init_Vertex_Buffer();

		grav.ResetGravity(6.0f, -6.0f);
	}

}
