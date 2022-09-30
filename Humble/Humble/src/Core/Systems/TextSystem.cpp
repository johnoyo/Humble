#include "TextSystem.h"
#include "../SystemsHeader.h"

namespace HBL {

	void TextSystem::Start()
	{
		Filter(Globals::entities, "Text", "TextTransform");

		// Import SDF data
		SDF_Importer("res/textures/testFont.fnt");

		// Load texture font atlas
		TextureManager::Load_Texture("res/textures/testFont.png");

		// Calculate buffer size for text vertex buffer
		uint32_t buffer_size = 0;
		For_Each([&](IEntity& entt)
		{
			Component::Text& text = GET_COMPONENT(Text, entt);
			buffer_size += text.text.size();
		});

		// Add another batch for text rendering
		const glm::mat4& vpMatrix = GlobalSystems::cameraSystem.Get_View_Projection_Matrix();
		Renderer::Get().AddBatch("res/shaders/Basic.shader", (buffer_size * 4), vpMatrix);
		
		// Retrieve vertex buffer for text
		VertexBuffer& buffer = Renderer::Get().GetVertexBuffer(1);

		For_Each([&](IEntity& entt)
		{
			Component::Text& text = GET_COMPONENT(Text, entt);
			Component::TextTransform& textTransform = GET_COMPONENT(TextTransform, entt);

			uint32_t prevIndex = INVALID_INDEX;

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
					int indx = Renderer::Get().Draw_Quad(1, tTr, sdfData[sdfIndex].width, sdfData[sdfIndex].height);

					// Retrieve font atlas texture id
					float id = TextureManager::Find("res/textures/testFont.png");

					// Calculate some offsets
					float line_width = (TextureManager::GetTextureSize().at(id).x / sdfData[sdfIndex].width);
					float line_height = (TextureManager::GetTextureSize().at(id).y / sdfData[sdfIndex].height);
					float line_offset = line_height - 1.0f;

					// Calculate letter position
					float x = (GetPositionX(line_width, sdfIndex, id));
					float y = (GetPositionY(line_height, sdfIndex, id));

					// Letter position and coordinates inside the font atlas
					glm::vec2 coords = glm::vec2(x, line_offset - y);
					glm::vec2 size = glm::vec2(sdfData[sdfIndex].width, sdfData[sdfIndex].height);

					// Update texture from font atlas info
					buffer.Update_Material_On_Quad(indx, text.color, id, coords, TextureManager::GetTextureSize().at(id), size);

					// Store previous index
					prevIndex = sdfIndex;
				}
			}
		});

		Renderer::Get().Bind(1);
		Renderer::Get().Invalidate(1);
		Renderer::Get().UnBind();
	}

	void TextSystem::Run(float dt)
	{
	}

	void TextSystem::Clear()
	{
	}

	float TextSystem::GetPositionX(float position, uint32_t sdfIndex, float id)
	{
		return (sdfData[sdfIndex].xCoord / (TextureManager::GetTextureSize().at(id).x / position));
	}

	float TextSystem::GetPositionY(float position, uint32_t sdfIndex, float id)
	{
		return (sdfData[sdfIndex].yCoord / (TextureManager::GetTextureSize().at(id).y / position));
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
