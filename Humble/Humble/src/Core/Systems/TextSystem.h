#pragma once
#include "../ISystem.h"
#include "../Rendering/Renderer.h"
#include <fstream>
#include <sstream>
#include <iterator>
#include <vector>
#include <algorithm>

namespace HBL {

	struct SDFData {
		int code;
		int xCoord;
		int yCoord;
		int width;
		int height;
		int xOffset;
		int yOffset;
		int xAdvance;
	};

	#define INVALID_INDEX 99999

	class HBL_API TextSystem final : public ISystem{
	public:
		virtual void Start() override;
		virtual void Run() override;
		virtual void Clear() override;

		float GetPositionX(float position, uint32_t sdfIndex, float id);
		float GetPositionY(float position, uint32_t sdfIndex, float id);
		uint32_t GetLetterIndex(char c);
		void SDF_Importer(const std::string& path);
		std::vector<SDFData> sdfData;
		float cursorPosition = 0.0f;
	};

}