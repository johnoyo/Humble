#include "LevelSystem.h"

void LevelSystem::LoadLevel(const std::string& level_path, ScriptingSystem& scr, GravitySystem& grav, RenderingSystem& rend, VertexBuffer& vertex_buffer, IndexBuffer& index_buffer, Entity::BaseEntity background)
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
		} else if (c == 'G') {
			p.push_back({ i,j,1.0f });
		} else if (c == 'C') {
			p.push_back({ i,j,3.0f });
		} else if (c == 'R') {
			p.push_back({ i,j,15.0f });
		} else if (c == 'E') {
			p.push_back({ i,j,4.0f });
		} else if (c == '>') {
			p.push_back({ i,j,8.0f });
		} else if (c == 'P') {
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
	for (unsigned int i = 0; i < entities.size(); i++) {
		Entity::BaseEntity entt = entities.at(i);
		if (entt.Transform != -1) {
			Transform.at(entt.Transform).Static = true;
			Transform.at(entt.Transform).Enabled = false;
		}
		if (entt.Material != -1) {
			Material.at(entt.Material).texture = "-";
			Material.at(entt.Material).subTexture.path = "-";
			Material.at(entt.Material).Enabled = false;
		}
		if (entt.Script != -1) {
			Script.at(entt.Script).Enabled = false;
		}
		if (entt.CollisionBox != -1) {
			CollisionBox.at(entt.CollisionBox).Enabled = false;
		}
		if (entt.Shadow != -1) {
			Shadow.at(entt.Shadow).Enabled = false;
		}
	}

	// Set up background
	ecs.GetComponent<Component::Transform>(background.Transform, Transform).position.x = 0.0f;
	ecs.GetComponent<Component::Transform>(background.Transform, Transform).position.y = 0.0f;
	ecs.GetComponent<Component::Transform>(background.Transform, Transform).scale.x = 1920.0f;
	ecs.GetComponent<Component::Transform>(background.Transform, Transform).scale.y = 1080.0f;
	ecs.GetComponent<Component::Material>(background.Material, Material).Enabled = true;
	ecs.GetComponent<Component::Material>(background.Material, Material).texture = "res/textures/brickWall_2.jpg";
	ecs.GetComponent<Component::Transform>(background.Transform, Transform).Static = false;
	ecs.GetComponent<Component::Transform>(background.Transform, Transform).Enabled = true;

	// Re enable lvlHandler
	ecs.GetComponent<Component::Script>(lvlHandler.Script, Script).Enabled = true;

	// Update the position of the entities
	for (unsigned int i = 0; i < p.size(); i++) {
		if (i == s) continue;
		if (p.at(i).k == 1.0f) {
			ecs.GetComponent<Component::Transform>(level[level_index].Transform, Transform).scale.x = 30.0f;
			ecs.GetComponent<Component::Transform>(level[level_index].Transform, Transform).scale.y = 30.0f;
			ecs.GetComponent<Component::Transform>(level[level_index].Transform, Transform).position.x = p.at(i).j * ecs.GetComponent<Component::Transform>(level[level_index].Transform, Transform).scale.x;
			ecs.GetComponent<Component::Transform>(level[level_index].Transform, Transform).position.y = p.at(i).i * ecs.GetComponent<Component::Transform>(level[level_index].Transform, Transform).scale.y;
			ecs.GetComponent<Component::Transform>(level[level_index].Transform, Transform).Static = true;
			ecs.GetComponent<Component::Transform>(level[level_index].Transform, Transform).Enabled = true;
			ecs.GetComponent<Component::Material>(level[level_index].Material, Material).Enabled = true;
			ecs.GetComponent<Component::Material>(level[level_index].Material, Material).texture = "res/textures/coinA.png";

			level_index++;
		} 
		else if (p.at(i).k == 6.0f) {
			ecs.GetComponent<Component::Transform>(wall[wall_index].Transform, Transform).scale.x = 30.0f;
			ecs.GetComponent<Component::Transform>(wall[wall_index].Transform, Transform).scale.y = 30.0f;
			ecs.GetComponent<Component::Transform>(wall[wall_index].Transform, Transform).position.x = p.at(i).j * ecs.GetComponent<Component::Transform>(wall[wall_index].Transform, Transform).scale.x;
			ecs.GetComponent<Component::Transform>(wall[wall_index].Transform, Transform).position.y = p.at(i).i * ecs.GetComponent<Component::Transform>(wall[wall_index].Transform, Transform).scale.y;
			ecs.GetComponent<Component::Transform>(wall[wall_index].Transform, Transform).Static = true;
			ecs.GetComponent<Component::Transform>(wall[wall_index].Transform, Transform).Enabled = true;
			ecs.GetComponent<Component::CollisionBox>(wall[wall_index].CollisionBox, CollisionBox).Enabled = true;
			ecs.GetComponent<Component::Material>(wall[wall_index].Material, Material).Enabled = true;
			ecs.GetComponent<Component::Material>(wall[wall_index].Material, Material).texture = "res/textures/brick_3.png";

			wall_index++;
		}
		else if (p.at(i).k == 4.0f) {
			ecs.GetComponent<Component::Transform>(enemy.Transform, Transform).scale.x = 30.0f;
			ecs.GetComponent<Component::Transform>(enemy.Transform, Transform).scale.y = 30.0f;
			ecs.GetComponent<Component::Transform>(enemy.Transform, Transform).position.x = p.at(i).j * ecs.GetComponent<Component::Transform>(enemy.Transform, Transform).scale.x;
			ecs.GetComponent<Component::Transform>(enemy.Transform, Transform).position.y = p.at(i).i * ecs.GetComponent<Component::Transform>(enemy.Transform, Transform).scale.y;
			ecs.GetComponent<Component::Transform>(enemy.Transform, Transform).Static = false;
			ecs.GetComponent<Component::Transform>(enemy.Transform, Transform).Enabled = true;
			ecs.GetComponent<Component::CollisionBox>(enemy.CollisionBox, CollisionBox).Enabled = true;
			ecs.GetComponent<Component::Script>(enemy.Script, Script).Enabled = true;
			ecs.GetComponent<Component::Material>(enemy.Material, Material).Enabled = true;
			GET_COMPONENT(Shadow, enemy).Enabled = true;
			enemy_index++;
		}
	}

	std::cout << "lvl indx: " << level_index << "\n";
	std::cout << "wall indx: " << wall_index << "\n";
	std::cout << "p size: " << p.size() << "\n";

	// Upadate the position of the player last
	ecs.GetComponent<Component::Transform>(player.Transform, Transform).scale.x = 29.0f;
	ecs.GetComponent<Component::Transform>(player.Transform, Transform).scale.y = 29.0f;
	ecs.GetComponent<Component::Transform>(player.Transform, Transform).position.x = p.at(s).j * ecs.GetComponent<Component::Transform>(player.Transform, Transform).scale.x;
	ecs.GetComponent<Component::Transform>(player.Transform, Transform).position.y = p.at(s).i * ecs.GetComponent<Component::Transform>(player.Transform, Transform).scale.y;
	ecs.GetComponent<Component::Transform>(player.Transform, Transform).Static = false;
	ecs.GetComponent<Component::Transform>(player.Transform, Transform).Enabled = true;
	ecs.GetComponent<Component::CollisionBox>(player.CollisionBox, CollisionBox).Enabled = true;
	ecs.GetComponent<Component::Script>(player.Script, Script).Enabled = true;
	ecs.GetComponent<Component::Material>(player.Material, Material).Enabled = true;

	// Recalculate all collision boxes
	for (unsigned int i = 0; i < entities.size(); i++) {
		if (entities.at(i).CollisionBox != -1 && entities.at(i).Transform != -1) {
			if (CollisionBox.at(entities.at(i).CollisionBox).Enabled) {
				CollisionBox.at(entities.at(i).CollisionBox).tl.x = Transform.at(entities.at(i).Transform).position.x - Transform.at(entities.at(i).Transform).scale.x / 2.0f;
				CollisionBox.at(entities.at(i).CollisionBox).tl.y = Transform.at(entities.at(i).Transform).position.y + Transform.at(entities.at(i).Transform).scale.y / 2.0f;

				CollisionBox.at(entities.at(i).CollisionBox).tr.x = Transform.at(entities.at(i).Transform).position.x + Transform.at(entities.at(i).Transform).scale.x / 2.0f;
				CollisionBox.at(entities.at(i).CollisionBox).tr.y = Transform.at(entities.at(i).Transform).position.y + Transform.at(entities.at(i).Transform).scale.y / 2.0f;

				CollisionBox.at(entities.at(i).CollisionBox).br.x = Transform.at(entities.at(i).Transform).position.x + Transform.at(entities.at(i).Transform).scale.x / 2.0f;
				CollisionBox.at(entities.at(i).CollisionBox).br.y = Transform.at(entities.at(i).Transform).position.y - Transform.at(entities.at(i).Transform).scale.y / 2.0f;

				CollisionBox.at(entities.at(i).CollisionBox).bl.x = Transform.at(entities.at(i).Transform).position.x - Transform.at(entities.at(i).Transform).scale.x / 2.0f;
				CollisionBox.at(entities.at(i).CollisionBox).bl.y = Transform.at(entities.at(i).Transform).position.y - Transform.at(entities.at(i).Transform).scale.y / 2.0f;
			}
		}
	}

	is.close();

	rend.Init_Vertex_Buffer();

	current_level++;

	grav.ResetGravity(6.0f, -6.0f);
	scr.Start(current_level);

}
