#include "TextureManager.h"
#include "../GlobalSystems.h"

namespace HBL {

	void TextureManager::IInitTransparentTexture()
	{
		uint32_t whiteTextureID;

		glCreateTextures(GL_TEXTURE_2D, 1, &whiteTextureID);
		glBindTexture(GL_TEXTURE_2D, whiteTextureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		uint32_t color = 0xffffffff;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &color);

		std::string str = "-";
		m_TextureMap.push_back(str);
		m_TextureSize.push_back({ 1.0f, 1.0f });
		m_TextureSlot[m_CurrentIndex++] = whiteTextureID;
	}

	void TextureManager::ILoadTexture(const std::string& path)
	{
		int w, h, bits;

		stbi_set_flip_vertically_on_load(1);
		auto* pixels = stbi_load(path.c_str(), &w, &h, &bits, STBI_rgb_alpha);
		assert(pixels);
		uint32_t tex_id;

		glCreateTextures(GL_TEXTURE_2D, 1, &tex_id);
		glBindTexture(GL_TEXTURE_2D, tex_id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

		stbi_image_free(pixels);

		m_TextureMap.push_back(path);
		m_TextureSize.push_back({ (float)w, (float)h });
		m_TextureSlot[m_CurrentIndex++] = tex_id;
		if (m_CurrentIndex == 31) std::cout << "NOTE: You have reached the max amount of textures(32)\n";
		assert(m_CurrentIndex < 32);
	}

	float TextureManager::IFind(const std::string& path)
	{
		for (int i = 0; i < m_TextureMap.size(); i++) {
			if (path == m_TextureMap.at(i)) return (float)i;
		}
		std::cerr << "Error! Could not find the file specified (" << path << "). Loading it from scratch!" << "\n";
		ILoadTexture(path);
		return (float)m_TextureMap.size() - 1;
	}

}
