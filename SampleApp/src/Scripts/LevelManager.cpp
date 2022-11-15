#include "LevelManager.h"

namespace HBL {

	void LevelManager::ILoadLevel(const std::string& level_path, ScriptingSystem& scr, GravitySystem& grav, SpriteRendererSystem& rend, VertexBuffer& vertex_buffer, IndexBuffer& index_buffer, IEntity& background, bool first)
	{
		std::ifstream is(level_path);

		char c;
		char acc[3];
		int i = 0, j = 0, s = 0, h = 0, w = 0;
		int index = 0;

		// Parse level dimensions
		while (is.get(c)) 
		{
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
		while (is.get(c)) 
		{
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
		for (uint32_t i = 0; i < Registry::Get().GetEntities().size(); i++) 
		{
			IEntity& entt = Registry::Get().GetEntities().at(i);
			if (Registry::Get().HasComponent<Component::Transform>(entt)) 
			{
				Registry::Get().GetComponent<Component::Transform>(entt).Static = true;
				Registry::Get().GetComponent<Component::Transform>(entt).Enabled = false;
			}
			if (Registry::Get().HasComponent<Component::SpriteRenderer>(entt))
			{
				Registry::Get().GetComponent<Component::SpriteRenderer>(entt).texture = "-";
				Registry::Get().GetComponent<Component::SpriteRenderer>(entt).Enabled = false;
			}
			if (Registry::Get().HasComponent<Component::Script>(entt))
			{
				Registry::Get().GetComponent<Component::Script>(entt).Enabled = false;
			}
			if (Registry::Get().HasComponent<Component::CollisionBox>(entt))
			{
				Registry::Get().GetComponent<Component::CollisionBox>(entt).Enabled = false;
			}
			if (Registry::Get().HasComponent<Component::Shadow>(entt))
			{
				Registry::Get().GetComponent<Component::Shadow>(entt).Enabled = false;
			}
			if (Registry::Get().HasComponent<Component::Gravity>(entt))
			{
				Registry::Get().GetComponent<Component::Gravity>(entt).Enabled = false;
			}
			if (Registry::Get().HasComponent<Component::Animation>(entt))
			{
				Registry::Get().GetComponent<Component::Animation>(entt).Enabled = false;
			}
		}

		// Set up background
		Component::Transform& backgroundTransform = Registry::Get().GetComponent<Component::Transform>(background);
		Component::SpriteRenderer& backgroundMaterial = Registry::Get().GetComponent<Component::SpriteRenderer>(background);

		backgroundTransform.position.x = 0.0f;
		backgroundTransform.position.y = 0.0f;
		backgroundTransform.scale.x = 1920.0f;
		backgroundTransform.scale.y = 1080.0f;
		backgroundTransform.Static = false;
		backgroundTransform.Enabled = true;
		backgroundMaterial.Enabled = true;
		backgroundMaterial.texture = "res/textures/brickWall_2.jpg";

		// Re enable lvlHandler
		Registry::Get().GetComponent<Component::Script>(lvlHandler).Enabled = true;

		// Update the position of the entities
		for (uint32_t i = 0; i < p.size(); i++) 
		{
			if (i == s) continue;
			if (p.at(i).k == 1.0f) 
			{
				Component::Transform& levelTransform = Registry::Get().GetComponent<Component::Transform>(level[level_index]);
				Component::SpriteRenderer& levelMaterial = Registry::Get().GetComponent<Component::SpriteRenderer>(level[level_index]);
				Component::CollisionBox& levelCollisionBox = Registry::Get().GetComponent<Component::CollisionBox>(level[level_index]);

				levelTransform.scale.x = 30.0f;
				levelTransform.scale.y = 30.0f;
				levelTransform.position.x = p.at(i).j * levelTransform.scale.x;
				levelTransform.position.y = p.at(i).i * levelTransform.scale.y;
				levelTransform.Static = true;
				levelTransform.Enabled = true;

				levelMaterial.Enabled = true;
				levelMaterial.texture = "res/textures/coinA.png";
				levelCollisionBox.Enabled = true;

				level_index++;
			}
			else if (p.at(i).k == 6.0f) 
			{
				Component::Transform& wallTransform = Registry::Get().GetComponent<Component::Transform>(wall[wall_index]);
				Component::SpriteRenderer& wallMaterial = Registry::Get().GetComponent<Component::SpriteRenderer>(wall[wall_index]);
				Component::CollisionBox& wallCollisionBox = Registry::Get().GetComponent<Component::CollisionBox>(wall[wall_index]);

				wallTransform.scale.x = 30.0f;
				wallTransform.scale.y = 30.0f;
				wallTransform.position.x = p.at(i).j * wallTransform.scale.x;
				wallTransform.position.y = p.at(i).i * wallTransform.scale.y;
				wallTransform.Static = true;
				wallTransform.Enabled = true;

				wallCollisionBox.Enabled = true;

				wallMaterial.Enabled = true;
				wallMaterial.texture = "res/textures/brick_3.png";

				if (!first)
					Registry::Get().GetComponent<Component::Shadow>(wall[wall_index]).Enabled = true;

				wall_index++;
			}
			else if (p.at(i).k == 4.0f) 
			{
				Component::Transform& enemyTransform = Registry::Get().GetComponent<Component::Transform>(enemy);
				Component::SpriteRenderer& enemyMaterial = Registry::Get().GetComponent<Component::SpriteRenderer>(enemy);
				Component::CollisionBox& enemyCollisionBox = Registry::Get().GetComponent<Component::CollisionBox>(enemy);

				enemyTransform.scale.x = 30.0f;
				enemyTransform.scale.y = 30.0f;
				enemyTransform.position.x = p.at(i).j * enemyTransform.scale.x;
				enemyTransform.position.y = p.at(i).i * enemyTransform.scale.y;
				enemyTransform.Static = false;
				enemyTransform.Enabled = true;

				enemyCollisionBox.Enabled = true;

				Registry::Get().GetComponent<Component::Script>(enemy).Enabled = true;

				enemyMaterial.Enabled = true;
				enemyMaterial.texture = "res/textures/enemy.png";

				Registry::Get().GetComponent<Component::Shadow>(enemy).Enabled = true;
				Registry::Get().GetComponent<Component::Gravity>(enemy).Enabled = true;
				enemy_index++;
			}
		}

		ENGINE_LOG("lvl indx: %d", level_index);
		ENGINE_LOG("wall indx: %d", wall_index);
		ENGINE_LOG("p size: %d", p.size());

		// Upadate the position of the player last
		Component::Transform& playerTransform = Registry::Get().GetComponent<Component::Transform>(player);
		Component::SpriteRenderer& playerMaterial = Registry::Get().GetComponent<Component::SpriteRenderer>(player);

		playerTransform.scale.x = 29.0f;
		playerTransform.scale.y = 29.0f;
		playerTransform.position.x = p.at(s).j * playerTransform.scale.x;
		playerTransform.position.y = p.at(s).i * playerTransform.scale.y;
		playerTransform.Static = false;
		playerTransform.Enabled = true;

		Registry::Get().GetComponent<Component::CollisionBox>(player).Enabled = true;

		Registry::Get().GetComponent<Component::Script>(player).Enabled = true;

		playerMaterial.Enabled = true;
		playerMaterial.texture = "res/textures/super_mario_tiles.png";

		Registry::Get().GetComponent<Component::Animation>(player).Enabled = true;

		// Recalculate all collision boxes
		for (uint32_t i = 0; i < Registry::Get().GetEntities().size(); i++) 
		{
			IEntity& entt = Registry::Get().GetEntities().at(i);

			if (Registry::Get().HasComponent<Component::CollisionBox>(entt) && Registry::Get().HasComponent<Component::Transform>(entt)) 
			{
				Component::CollisionBox& collisionBox = Registry::Get().GetComponent<Component::CollisionBox>(entt);
				if (collisionBox.Enabled)
				{
					Component::Transform& transform = Registry::Get().GetComponent<Component::Transform>(entt);

					collisionBox.tl.x = transform.position.x - transform.scale.x / 2.0f;
					collisionBox.tl.y = transform.position.y + transform.scale.y / 2.0f;
								 		
					collisionBox.tr.x = transform.position.x + transform.scale.x / 2.0f;
					collisionBox.tr.y = transform.position.y + transform.scale.y / 2.0f;
										
					collisionBox.br.x = transform.position.x + transform.scale.x / 2.0f;
					collisionBox.br.y = transform.position.y - transform.scale.y / 2.0f;
										
					collisionBox.bl.x = transform.position.x - transform.scale.x / 2.0f;
					collisionBox.bl.y = transform.position.y - transform.scale.y / 2.0f;
				}
			}
		}

		is.close();
		rend.InitVertexBuffer();
	//	grav.ResetGravity(1000.0f, -1000.0f);
	}

}
