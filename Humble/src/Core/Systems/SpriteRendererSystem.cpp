#include "SpriteRendererSystem.h"

void HBL::SpriteRendererSystem::Start()
{
	FUNCTION_PROFILE();

	Renderer::Get().AddBatch("res/shaders/Basic.shader", (Registry::Get().GetEntities().size() * 4) + (Registry::Get().GetArray<Component::Shadow>().size() * 12), SceneManager::Get().GetMainCamera());
	
	TextureManager::Get().InitTransparentTexture();

	Renderer::Get().GetVertexBuffer(0).Reset();

	Registry::Get().Group<Component::Transform, Component::SpriteRenderer>().ForEach([&](IEntity& entt)
	{
		Component::Transform& transform = Registry::Get().GetComponent<Component::Transform>(entt);
		Component::SpriteRenderer& sprite = Registry::Get().GetComponent<Component::SpriteRenderer>(entt);

		if (transform.Enabled)
		{
			sprite.bufferIndex = Renderer::Get().GetVertexBuffer(0).m_Index;
			transform.bufferIndex = Renderer::Get().GetVertexBuffer(0).m_Index;
			Renderer::Get().RegisterQuad(0, transform);
		}

		if (sprite.texture != "-")
		{
			if (sprite.pixelData != nullptr)
				TextureManager::Get().Find(sprite.texture, &sprite);
			else
				TextureManager::Get().Find(sprite.texture);
		}
	}).Run();
}

void HBL::SpriteRendererSystem::Run(float dt)
{
	//FUNCTION_PROFILE();

	Registry::Get().View<Component::SpriteRenderer>().ForEach([&](Component::SpriteRenderer& sprite)
	{
		if (sprite.Enabled)
		{
			if (sprite.pixelData != nullptr)
			{
				Renderer::Get().UpdateQuad(0, sprite.bufferIndex, sprite.color, TextureManager::Get().Find(sprite.texture, &sprite));
			}
			else if (sprite.coords == glm::vec2(-1.0f, -1.0f) && sprite.spriteSize == glm::vec2(-1.0f, -1.0f))
			{
				Renderer::Get().UpdateQuad(0, sprite.bufferIndex, sprite.color, TextureManager::Get().Find(sprite.texture));
			}
			else
			{
				float id = TextureManager::Get().Find(sprite.texture);
				glm::vec2& textureCoords = TextureManager::Get().GetTextureSize().at(id);
				Renderer::Get().UpdateQuad(0, sprite.bufferIndex, sprite.color, id, sprite.coords, textureCoords, sprite.spriteSize);
			}
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
