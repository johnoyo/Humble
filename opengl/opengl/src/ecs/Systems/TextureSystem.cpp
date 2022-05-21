#include "TextureSystem.h"

void TextureSystem::Init_Transparent_Texture()
{
	unsigned int white_texture_id;

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
	unsigned int tex_id;

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
	Init_Transparent_Texture();

	for (unsigned int i = 0; i < Material.size(); i++) {
		if (Material.at(i).texture != "-") Load_Texture(Material.at(i).texture);
	}
}

void TextureSystem::Run(RenderingSystem& rend)
{
	unsigned int indx = 0;
	for (unsigned int i = 0; i < Material.size(); i++) {
		if (Material.at(i).Enabled) {
			if (Material.at(i).subTexture.path == "-") {
				rend.Get_Vertex_Buffer().Update_Material_On_Quad(indx, Material.at(i).color, Find(Material.at(i).texture));
			} else {
				rend.Get_Vertex_Buffer().Update_Material_On_Quad(indx, Material.at(i).color, Find(Material.at(i).subTexture.path), Material.at(i).subTexture.coords, size.at(Find(Material.at(i).subTexture.path)), Material.at(i).subTexture.sprite_size);
			}
			indx += 4;
		}
	}

	for (unsigned int i = 0; i < current_index; i++) {
		GLCall(glBindTextureUnit(i, texture_slot[i]));
	}
}

void TextureSystem::Clear()
{
	GLCall(glDeleteTextures(32, texture_slot));
}

