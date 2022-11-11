#include "SpriteRendererSystem.h"
#include "../GlobalSystems.h"

void HBL::SpriteRendererSystem::Start()
{
	FUNCTION_PROFILE();

	const glm::mat4& vpMatrix = GlobalSystems::cameraSystem.Get_View_Projection_Matrix();
	Renderer::Get().AddBatch("res/shaders/Basic.shader", (Registry::Get().GetEntities().size() * 4) + (Registry::Get().GetArray<Component::Shadow>().size() * 12), vpMatrix);
	InitVertexBuffer();

	TextureManager::InitTransparentTexture();

	ForEach([&](IEntity& entt)
	{
		Component::SpriteRenderer& sprite = Registry::Get().GetComponent<Component::SpriteRenderer>(entt);
		if (sprite.texture != "-")
			TextureManager::LoadTexture(sprite.texture);
	}).Run();
}

void HBL::SpriteRendererSystem::Run(float dt)
{
	//FUNCTION_PROFILE();

	VertexBuffer& buffer = Renderer::Get().GetVertexBuffer(0);

	uint32_t indx = 0;

	ForEach([&](IEntity& entt)
	{
		Component::SpriteRenderer& sprite = Registry::Get().GetComponent<Component::SpriteRenderer>(entt);
		if (sprite.Enabled)
		{
			if (sprite.coords == glm::vec2(-1.0f, -1.0f) && sprite.sprite_size == glm::vec2(-1.0f, -1.0f))
			{
				buffer.UpdateMaterialOnQuad(indx, sprite.color, TextureManager::Find(sprite.texture));
			}
			else
			{
				float id = TextureManager::Find(sprite.texture);
				buffer.UpdateMaterialOnQuad(indx, sprite.color, id, sprite.coords, TextureManager::GetTextureSize().at(id), sprite.sprite_size);
			}
			indx += 4;
		}
	}).Run();

	for (uint32_t i = 0; i < TextureManager::GetTextureIndex(); i++)
	{
		glBindTextureUnit(i, TextureManager::GetTextureSlot()[i]);
	}
}

void HBL::SpriteRendererSystem::Clear()
{
	FUNCTION_PROFILE();

	TextureManager::GetTextureMap().clear();
	TextureManager::GetTextureSize().clear();

	TextureManager::GetTextureIndex() = 0;

	Registry::Get().GetArray<Component::SpriteRenderer>().clear();
	glDeleteTextures(32, TextureManager::GetTextureSlot());
}

void HBL::SpriteRendererSystem::InitVertexBuffer()
{
	Renderer::Get().GetVertexBuffer(0).Reset();

	ENGINE_LOG("Transform size: %d", Registry::Get().GetArray<Component::Transform>().size());

	Filter<Component::Transform, Component::SpriteRenderer>().ForEach([&](IEntity& entt)
	{
		Component::Transform& transform = Registry::Get().GetComponent<Component::Transform>(entt);
		if (transform.Enabled)
		{
			transform.bufferIndex = Renderer::Get().GetVertexBuffer(0).m_Index;
			Renderer::Get().DrawQuad(0, transform);
		}
	}).Run();

	Renderer::Get().Bind(0);
	Renderer::Get().Invalidate(0);
	Renderer::Get().UnBind();
}
