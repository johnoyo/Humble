#include "MaterialSystem.h"
#include "../SystemsHeader.h"

namespace HBL {

	void MaterialSystem::Start()
	{
		FUNCTION_PROFILE();

		TextureManager::Init_Transparent_Texture();

		for (uint32_t i = 0; i < Globals::Material.size(); i++) 
		{
			if (Globals::Material.at(i).texture != "-") 
				TextureManager::Load_Texture(Globals::Material.at(i).texture);
		}
	}

	void MaterialSystem::Run()
	{
		//FUNCTION_PROFILE();
		
		VertexBuffer& buffer = Renderer::Get().GetVertexBuffer(0);

		uint32_t indx = 0;
		for (uint32_t i = 0; i < Globals::Material.size(); i++) 
		{
			Component::Material& material = Globals::Material.at(i);
			if (material.Enabled) 
			{
				if (material.coords == glm::vec2( -1.0f, -1.0f ) && material.sprite_size == glm::vec2(-1.0f, -1.0f))
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
		}

		for (uint32_t i = 0; i < TextureManager::GetTextureIndex(); i++)
		{
			GLCall(glBindTextureUnit(i, TextureManager::GetTextureSlot()[i]));
		}
	}

	void MaterialSystem::Clear()
	{
		FUNCTION_PROFILE();

		TextureManager::GetTextureMap().clear();
		TextureManager::GetTextureSize().clear();
		TextureManager::GetTextureIndex() = 0;
		Globals::Material.clear();
		GLCall(glDeleteTextures(32, TextureManager::GetTextureSlot()));
	}

}
