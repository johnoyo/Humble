#include "TextureSystem.h"

void TextureSystem::Init_Transparent_Texture()
{
	uint32_t white_texture_id;

	GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &white_texture_id));
	GLCall(glBindTexture(GL_TEXTURE_2D, white_texture_id));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	uint32_t color = 0xffffffff;
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &color));

	std::string str = "-";
	map.push_back(str);
	size.push_back({ 1.0f, 1.0f });
	texture_slot[current_index++] = white_texture_id;
}

void TextureSystem::Load_Texture(const std::string& path)
{
	int w, h, bits;

	stbi_set_flip_vertically_on_load(1);
	auto* pixels = stbi_load(path.c_str(), &w, &h, &bits, STBI_rgb_alpha);
	assert(pixels);
	uint32_t tex_id;

	GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &tex_id));
	GLCall(glBindTexture(GL_TEXTURE_2D, tex_id));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels));

	stbi_image_free(pixels);

	map.push_back(path);
	size.push_back({ (float)w, (float)h });
	texture_slot[current_index++] = tex_id;
	if (current_index == 31) std::cout << "NOTE: You have reached the max amount of textures(32)\n";
	assert(current_index < 32);
}

float TextureSystem::Find(const std::string& path)
{
	for (int i = 0; i < map.size(); i++) {
		if (path == map.at(i)) return (float)i;
	}
	std::cerr << "Error! Could not find the file specified (" << path << "). Loading it from scratch!" << "\n";
	Load_Texture(path);
	return (float)map.size() - 1;
}

void TextureSystem::Start()
{
	ENGINE_PROFILE("TextureSystem::Start");
	Init_Transparent_Texture();

	for (uint32_t i = 0; i < Material.size(); i++) {
		if (Material.at(i).texture != "-") Load_Texture(Material.at(i).texture);
	}
}

void TextureSystem::Run(VertexBuffer& buffer)
{
	//ENGINE_PROFILE("TextureSystem::Run");

	uint32_t indx = 0;
	for (uint32_t i = 0; i < Material.size(); i++) {
		Component::Material& material = Material.at(i);
		if (material.Enabled) {
			if (material.subTexture.path == "-") {
				//if (material.old_texture != material.texture) {
					//material.old_texture = material.texture;
					buffer.Update_Material_On_Quad(indx, material.color, Find(material.texture));
				//}
			} else {
				float id = Find(material.subTexture.path);
				buffer.Update_Material_On_Quad(indx, material.color, id, material.subTexture.coords, size.at(id), material.subTexture.sprite_size);
			}
			indx += 4;
		}
	}

	for (uint32_t i = 0; i < current_index; i++) {
		GLCall(glBindTextureUnit(i, texture_slot[i]));
	}
}

void TextureSystem::Clear()
{
	map.clear();
	size.clear();
	current_index = 0;
	Material.clear();
	GLCall(glDeleteTextures(32, texture_slot));
}

