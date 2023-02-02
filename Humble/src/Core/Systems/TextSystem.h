#pragma once

#include <vector>
#include <fstream>
#include <sstream>
#include <iterator>
#include <algorithm>

#include "../ECS/ISystem.h"
#include "../Rendering/Renderer.h"

namespace HBL 
{
	struct SDFData 
	{
		int code;
		int xCoord;
		int yCoord;
		int width;
		int height;
		int xOffset;
		int yOffset;
		int xAdvance;
	};

	#define INVALID_INDEX UINT32_MAX

	class HBL_API TextSystem final : public ISystem
	{
	public:
		virtual void Start() override;
		virtual void Run(float dt) override;
		virtual void Clear() override;

	private:
		float GetPositionX(float position, uint32_t sdfIndex, float id);
		float GetPositionY(float position, uint32_t sdfIndex, float id);
		uint32_t GetLetterIndex(char c);
		void SDF_Importer(const std::string& path);
		std::vector<SDFData> sdfData;
		float cursorPosition = 0.0f;
	};
}