#include "SpriteRendererSystem.h"

void HBL::SpriteRendererSystem::Start()
{
	FUNCTION_PROFILE();

	Renderer::Get().AddBatch("res/shaders/Basic.shader", (Registry::Get().GetEntities().size() * 4) + (Registry::Get().GetArray<Component::Shadow>().size() * 12), Globals::Camera);
	InitVertexBuffer();

	TextureManager::Get().InitTransparentTexture();

	Filter<Component::Transform, Component::SpriteRenderer>().ForEach([&](IEntity& entt)
	{
		Component::SpriteRenderer& sprite = Registry::Get().GetComponent<Component::SpriteRenderer>(entt);
		if (sprite.texture != "-")
			TextureManager::Get().LoadTexture(sprite.texture);
	}).Run();
}

void HBL::SpriteRendererSystem::Run(float dt)
{
	//FUNCTION_PROFILE();

	VertexBuffer& buffer = Renderer::Get().GetVertexBuffer(0);

	uint32_t indx = 0;

	Filter<Component::Transform, Component::SpriteRenderer>().ForEach([&](IEntity& entt)
	{
		Component::SpriteRenderer& sprite = Registry::Get().GetComponent<Component::SpriteRenderer>(entt);
		if (sprite.Enabled)
		{
			if (sprite.coords == glm::vec2(-1.0f, -1.0f) && sprite.sprite_size == glm::vec2(-1.0f, -1.0f))
			{
				buffer.UpdateMaterialOnQuad(indx, sprite.color, TextureManager::Get().Find(sprite.texture));
			}
			else
			{
				float id = TextureManager::Get().Find(sprite.texture);
				buffer.UpdateMaterialOnQuad(indx, sprite.color, id, sprite.coords, TextureManager::Get().GetTextureSize().at(id), sprite.sprite_size);
			}
			indx += 4;
		}
	}).Run();

	for (uint32_t i = 0; i < TextureManager::Get().GetTextureIndex(); i++)
	{
		glBindTextureUnit(i, TextureManager::Get().GetTextureSlot()[i]);
	}
}

void HBL::SpriteRendererSystem::Clear()
{
	FUNCTION_PROFILE();

	TextureManager::Get().GetTextureMap().clear();
	TextureManager::Get().GetTextureSize().clear();

	TextureManager::Get().GetTextureIndex() = 0;

	Registry::Get().ClearArray<Component::SpriteRenderer>();
	glDeleteTextures(32, TextureManager::Get().GetTextureSlot());
}

void HBL::SpriteRendererSystem::InitVertexBuffer()
{
	FUNCTION_PROFILE();

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
