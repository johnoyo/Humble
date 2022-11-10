#include "MaterialSystem.h"
#include "../GlobalSystems.h"

namespace HBL {

	void MaterialSystem::Start()
	{
		FUNCTION_PROFILE();

		TextureManager::Init_Transparent_Texture();

		Filter<Component::Material>().ForEach([&](IEntity& entt)
		{
			Component::Material& material = Globals::s_Registry.GetComponent<Component::Material>(entt);
			if (material.texture != "-")
				TextureManager::Load_Texture(material.texture);
		}).Run();
	}

	void MaterialSystem::Run(float dt)
	{
		//FUNCTION_PROFILE();
		
		VertexBuffer& buffer = Renderer::Get().GetVertexBuffer(0);

		uint32_t indx = 0;

		ForEach([&](IEntity& entt)
		{
			Component::Material& material = Globals::s_Registry.GetComponent<Component::Material>(entt);
			if (material.Enabled)
			{
				if (material.coords == glm::vec2(-1.0f, -1.0f) && material.sprite_size == glm::vec2(-1.0f, -1.0f))
				{
					buffer.Update_Material_On_Quad(indx, material.color, TextureManager::Find(material.texture));
				}
				else
				{
					float id = TextureManager::Find(material.texture);
					buffer.Update_Material_On_Quad(indx, material.color, id, material.coords, TextureManager::GetTextureSize().at(id), material.sprite_size);
				}
				indx += 4;
			}
		}).Run();

		for (uint32_t i = 0; i < TextureManager::GetTextureIndex(); i++)
		{
			glBindTextureUnit(i, TextureManager::GetTextureSlot()[i]);
		}
	}

	void MaterialSystem::Clear()
	{
		FUNCTION_PROFILE();

		TextureManager::GetTextureMap().clear();
		TextureManager::GetTextureSize().clear();

		TextureManager::GetTextureIndex() = 0;

		Globals::s_Registry.GetArray<Component::Material>().clear();
		glDeleteTextures(32, TextureManager::GetTextureSlot());
	}

}
