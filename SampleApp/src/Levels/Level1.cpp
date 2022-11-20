#include "Level1.h"

void HBL::Level1::OnAttach()
{
	// Enroll entities.
	Registry::Get().EnrollEntity(background, "Background");
	Registry::Get().EnrollEntity(player, "Player");
	Registry::Get().EnrollEntity(enemy, "Enemy");
	Registry::Get().EnrollEntity(camera, "Camera");
	Registry::Get().EnrollEntity(lvlHandler);

	for (uint32_t i = 0; i < 400; i++)
		Registry::Get().EnrollEntity(wall[i]);

	for (uint32_t i = 0; i < 100; i++)
		Registry::Get().EnrollEntity(level[i]);

	// Add components.
	Registry::Get().AddComponent<Component::SpriteRenderer>(background);

	Registry::Get().AddComponent<Component::Script>(player);
	Registry::Get().AddComponent<Component::SpriteRenderer>(player);
	Registry::Get().AddComponent<Component::CollisionBox>(player);
	Registry::Get().AddComponent<Component::Animation>(player);

	Registry::Get().AddComponent<Component::Script>(enemy);
	Registry::Get().AddComponent<Component::SpriteRenderer>(enemy);
	Registry::Get().AddComponent<Component::CollisionBox>(enemy);
	Registry::Get().AddComponent<Component::Gravity>(enemy);
	Registry::Get().AddComponent<Component::Shadow>(enemy);

	Registry::Get().AddComponent<Component::Camera>(camera);

	Registry::Get().AddComponent<Component::Script>(lvlHandler);

	for (uint32_t i = 0; i < 400; i++)
	{
		Registry::Get().AddComponent<Component::Shadow>(wall[i]);
		Registry::Get().AddComponent<Component::CollisionBox>(wall[i]);
		Registry::Get().AddComponent<Component::SpriteRenderer>(wall[i]);
	}

	for (uint32_t i = 0; i < 100; i++)
	{
		Registry::Get().AddComponent<Component::CollisionBox>(level[i]);
		Registry::Get().AddComponent<Component::SpriteRenderer>(level[i]);
	}

	// Initialize components.
	Registry::Get().GetComponent<Component::Script>(player).script.push_back(new PlayerScript());
	Registry::Get().GetComponent<Component::Script>(enemy).script.push_back(new EnemyScript());
	Registry::Get().GetComponent<Component::Script>(lvlHandler).script.push_back(new LevelHandlerScript());

	Registry::Get().GetComponent<Component::Transform>(player).Static = false;
	Registry::Get().GetComponent<Component::Transform>(enemy).Static = false;
	Registry::Get().GetComponent<Component::Transform>(background).Static = false;

	Registry::Get().GetComponent<Component::Shadow>(enemy).source = &player;
	Registry::Get().GetComponent<Component::Gravity>(enemy).force = 1000.0f;
	Registry::Get().GetComponent<Component::Gravity>(enemy).threshold = -1000.0f;

	Registry::Get().GetComponent<Component::Camera>(camera).projection = glm::ortho(
		0.0f, Systems::Window.GetWidth(),
		0.0f, Systems::Window.GetHeight(),
		-1.0f, 1.0f);

	for (uint32_t i = 0; i < 400; i++)
	{
		Registry::Get().GetComponent<Component::Transform>(wall[i]).Static = true;
		Registry::Get().GetComponent<Component::Shadow>(wall[i]).source = &player;

		Registry::Get().GetComponent<Component::Shadow>(wall[i]).Enabled = false;
		Registry::Get().GetComponent<Component::Transform>(wall[i]).Enabled = false;
		Registry::Get().GetComponent<Component::CollisionBox>(wall[i]).Enabled = false;
		Registry::Get().GetComponent<Component::SpriteRenderer>(wall[i]).Enabled = false;
	}

	for (uint32_t i = 0; i < 100; i++)
	{
		Registry::Get().GetComponent<Component::Transform>(level[i]).Static = true;
		Registry::Get().GetComponent<Component::Transform>(level[i]).Enabled = false;
		Registry::Get().GetComponent<Component::CollisionBox>(level[i]).Enabled = false;
		Registry::Get().GetComponent<Component::SpriteRenderer>(level[i]).Enabled = false;
	}
}

void HBL::Level1::OnCreate()
{
	ILoadLevel("res/levels/test2.txt");
}

void HBL::Level1::OnDetach()
{
}

void HBL::Level1::ILoadLevel(const std::string& level_path)
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

	int level_index = 0;
	int wall_index = 0;
	int enemy_index = 0;
	int collectible_index = 0;

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

	// Update the position of the player last
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

	is.close();
}
