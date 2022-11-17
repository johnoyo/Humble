#include "TextSystem.h"
#include "../Systems.h"

namespace HBL 
{
	void TextSystem::Start()
	{
		// Import SDF data
		SDF_Importer("res/textures/testFont.fnt");

		// Load texture font atlas
		TextureManager::Get().LoadTexture("res/textures/testFont.png");

		// Add another batch for text rendering (200 characters for each text component).
		Renderer::Get().AddBatch("res/shaders/Basic.shader", 200 * (Registry::Get().GetArray<Component::Text>().size() * 4), SceneManager::Get().GetMainCamera());
		
		// Retrieve vertex buffer for text
		VertexBuffer& buffer = Renderer::Get().GetVertexBuffer(1);

		Filter<Component::Text, Component::TextTransform>().ForEach([&](IEntity& entt)
		{
			Component::Text& text = Registry::Get().GetComponent<Component::Text>(entt);
			Component::TextTransform& textTransform = Registry::Get().GetComponent<Component::TextTransform>(entt);

			if (textTransform.screenSpace)
			{
				textTransform.position.x = Registry::Get().GetComponent<Component::Transform>(SceneManager::Get().GetMainCamera()).position.x + textTransform.sreenSpaceOffset.x;
				textTransform.position.y = Registry::Get().GetComponent<Component::Transform>(SceneManager::Get().GetMainCamera()).position.y + textTransform.sreenSpaceOffset.y;
			}

			uint32_t prevIndex = INVALID_INDEX;
			cursorPosition = 0.0f;

			if (text.Enabled)
			{
				const std::string& t = text.text;

				for (uint32_t i = 0; i < t.size(); i++)
				{
					// Retrieve index for SDF vector
					uint32_t sdfIndex = GetLetterIndex(t[i]);

					Component::TextTransform tTr = textTransform;

					// If its not the first letter calculate correct offset
					if (prevIndex != INVALID_INDEX)
						cursorPosition += ((sdfData[sdfIndex].xAdvance / 2.0f) * tTr.scale.x) + ((sdfData[prevIndex].xAdvance / 2.0f) * tTr.scale.x);

					// Move cursor and position current letter
					tTr.position.x += cursorPosition;

					// Draw the current letter as a new quad
					int index = Renderer::Get().DrawQuad(1, tTr, sdfData[sdfIndex].width * tTr.scale.x, sdfData[sdfIndex].height * tTr.scale.y);

					// Retrieve font atlas texture id
					float id = TextureManager::Get().Find("res/textures/testFont.png");

					// Calculate some offsets
					float line_width = (TextureManager::Get().GetTextureSize().at(id).x / sdfData[sdfIndex].width);
					float line_height = (TextureManager::Get().GetTextureSize().at(id).y / sdfData[sdfIndex].height);
					float line_offset = line_height - 1.0f;

					// Calculate letter position
					float x = (GetPositionX(line_width, sdfIndex, id));
					float y = (GetPositionY(line_height, sdfIndex, id));

					// Letter position and coordinates inside the font atlas
					glm::vec2 coords = glm::vec2(x, line_offset - y);
					glm::vec2 size = glm::vec2(sdfData[sdfIndex].width, sdfData[sdfIndex].height);

					// Update texture from font atlas info
					buffer.UpdateMaterialOnQuad(index, text.color, id, coords, TextureManager::Get().GetTextureSize().at(id), size);

					// Update position of current character
					buffer.UpdatePositionOnQuad(index, tTr.position, tTr.rotation, glm::vec3((float)sdfData[sdfIndex].width * tTr.scale.x, (float)sdfData[sdfIndex].height * tTr.scale.y, 1.0f));

					// Store previous index
					prevIndex = sdfIndex;

					textTransform.bufferIndex.push_back(index);
				}
			}
		}).Run();

		Renderer::Get().Bind(1);
		Renderer::Get().Invalidate(1);
		Renderer::Get().UnBind();
	}

	void TextSystem::Run(float dt)
	{
		// Retrieve vertex buffer for text
		VertexBuffer& buffer = Renderer::Get().GetVertexBuffer(1);

		bool invalidate = false;

		Filter<Component::Text, Component::TextTransform>().ForEach([&] (IEntity& entt)
		{
			Component::Text& text = Registry::Get().GetComponent<Component::Text>(entt);
			Component::TextTransform& textTransform = Registry::Get().GetComponent<Component::TextTransform>(entt);

			// If screen space text follow camera.
			if (textTransform.screenSpace)
			{
				textTransform.position.x = Registry::Get().GetComponent<Component::Transform>(SceneManager::Get().GetMainCamera()).position.x + textTransform.sreenSpaceOffset.x;
				textTransform.position.y = Registry::Get().GetComponent<Component::Transform>(SceneManager::Get().GetMainCamera()).position.y + textTransform.sreenSpaceOffset.y;
			}

			uint32_t prevIndex = INVALID_INDEX;
			cursorPosition = 0.0f;

			if (text.Enabled)
			{
				const std::string& t = text.text;

				for (uint32_t i = 0; i < t.size(); i++)
				{
					// Retrieve index for SDF vector
					uint32_t sdfIndex = GetLetterIndex(t[i]);

					Component::TextTransform tTr = textTransform;

					// If its not the first letter calculate correct offset
					if (prevIndex != INVALID_INDEX)
						cursorPosition += ((sdfData[sdfIndex].xAdvance / 2.0f) * tTr.scale.x) + ((sdfData[prevIndex].xAdvance / 2.0f) * tTr.scale.x);

					// Move cursor and position current letter
					tTr.position.x += cursorPosition;

					// Retrieve font atlas texture id
					float id = TextureManager::Get().Find("res/textures/testFont.png");

					// Register new quad if max numbers of currently available characters is exceeded.
					if (i >= textTransform.bufferIndex.size())
					{
						invalidate = true;

						// Draw the current letter as a new quad.
						int index = Renderer::Get().DrawQuad(1, tTr, sdfData[sdfIndex].width * tTr.scale.x, sdfData[sdfIndex].height * tTr.scale.y);

						textTransform.bufferIndex.push_back(index);
					}
					else if (textTransform.bufferIndex.size() > t.size() && i == t.size() - 1)
					{
						// Retrieve whitespace character
						uint32_t sdfIndexRemove = GetLetterIndex('\ ');

						// Calculate some offsets
						float line_width = (TextureManager::Get().GetTextureSize().at(id).x / sdfData[sdfIndexRemove].width);
						float line_height = (TextureManager::Get().GetTextureSize().at(id).y / sdfData[sdfIndexRemove].height);
						float line_offset = line_height - 1.0f;

						// Calculate letter position
						float x = (GetPositionX(line_width, sdfIndexRemove, id));
						float y = (GetPositionY(line_height, sdfIndexRemove, id));

						// Letter position and coordinates inside the font atlas
						glm::vec2 coords = glm::vec2(x, line_offset - y);
						glm::vec2 size = glm::vec2(sdfData[sdfIndexRemove].width, sdfData[sdfIndexRemove].height);

						// Remove left over characters
						for (uint32_t j = 0; j < (uint32_t)(textTransform.bufferIndex.size() - t.size()); j++)
						{
							// Update texture from font atlas info
							buffer.UpdateMaterialOnQuad(textTransform.bufferIndex[i + j + 1U], text.color, id, coords, TextureManager::Get().GetTextureSize().at(id), size);

							// Update position of current character
							buffer.UpdatePositionOnQuad(textTransform.bufferIndex[i + j + 1U], tTr.position, tTr.rotation, glm::vec3( (float)sdfData[sdfIndex].width * tTr.scale.x, (float)sdfData[sdfIndex].height * tTr.scale.y, 1.0f ));
						}
					}

					// Calculate some offsets
					float line_width = (TextureManager::Get().GetTextureSize().at(id).x / sdfData[sdfIndex].width);
					float line_height = (TextureManager::Get().GetTextureSize().at(id).y / sdfData[sdfIndex].height);
					float line_offset = line_height - 1.0f;

					// Calculate letter position
					float x = (GetPositionX(line_width, sdfIndex, id));
					float y = (GetPositionY(line_height, sdfIndex, id));

					// Letter position and coordinates inside the font atlas
					glm::vec2 coords = glm::vec2(x, line_offset - y);
					glm::vec2 size = glm::vec2(sdfData[sdfIndex].width, sdfData[sdfIndex].height);

					// Update texture from font atlas info
					buffer.UpdateMaterialOnQuad(textTransform.bufferIndex[i], text.color, id, coords, TextureManager::Get().GetTextureSize().at(id), size);

					// Update position of current character
					buffer.UpdatePositionOnQuad(textTransform.bufferIndex[i], tTr.position, tTr.rotation, glm::vec3((float)sdfData[sdfIndex].width * tTr.scale.x, (float)sdfData[sdfIndex].height * tTr.scale.y, 1.0f));

					// Store previous index
					prevIndex = sdfIndex;
				}
			}
		}).Run();

		if (invalidate)
		{
			Renderer::Get().Bind(1);
			Renderer::Get().Invalidate(1);
			Renderer::Get().UnBind();
		}
	}

	void TextSystem::Clear()
	{
		Registry::Get().ClearArray<Component::Text>();
		Registry::Get().ClearArray<Component::TextTransform>();
	}

	float TextSystem::GetPositionX(float position, uint32_t sdfIndex, float id)
	{
		return (sdfData[sdfIndex].xCoord / (TextureManager::Get().GetTextureSize().at(id).x / position));
	}

	float TextSystem::GetPositionY(float position, uint32_t sdfIndex, float id)
	{
		return (sdfData[sdfIndex].yCoord / (TextureManager::Get().GetTextureSize().at(id).y / position));
	}

	uint32_t TextSystem::GetLetterIndex(char c)
	{
		int character = c;
		for (uint32_t i = 0; i < sdfData.size(); i++)
		{
			if (character == sdfData[i].code)
				return i;
		}
		std::cout << "Error! Illegal character used.\n";
		return 35;
	}

	void TextSystem::SDF_Importer(const std::string& path)
	{
		std::ifstream is(path);
		std::string line;

		// Skip the first 3 lines since they dont contain usefull data
		for (int i = 0; i < 4; i++)
			std::getline(is, line);

		// In the 4th line erase all but numbers
		line.erase(std::remove_if(line.begin(), line.end(), ispunct), line.end());
		line.erase(std::remove_if(line.begin(), line.end(), isalpha), line.end());
		line.erase(std::remove_if(line.begin(), line.end(), isblank), line.end());
		line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());

		// Convert string to integer, this is the number of entries in the SDF
		int count = stoi(line);
		char space_char = ' ';

		// Parse SDF data to vector
		for (int i = 0; i < count; i++)
		{
			std::getline(is, line);
			std::vector<std::string> words{};

			std::stringstream sstream(line);
			std::string word;
			while (std::getline(sstream, word, space_char)) 
			{
				if (word.size() != 0)
				{
					// Erase unwanted characters from string and only leave the number with its sign
					word.erase(std::remove(word.begin(), word.end(), '='), word.cend());
					word.erase(std::remove_if(word.begin(), word.end(), isalpha), word.end());
					word.erase(std::remove_if(word.begin(), word.end(), isblank), word.end());
					word.erase(std::remove_if(word.begin(), word.end(), isspace), word.end());

					if (word.size() != 0)
						words.push_back(word);
				}
			}

			// Convert string data to integer and store them to vector
			sdfData.push_back({
				stoi(words[0]),
				stoi(words[1]),
				stoi(words[2]),
				stoi(words[3]),
				stoi(words[4]),
				stoi(words[5]),
				stoi(words[6]),
				stoi(words[7])
			});
		}

		is.close();

		return;
	}
}
