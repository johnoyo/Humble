#pragma once
#include "../ISystem.h"
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

	class HBL_API TextSystem final : public ISystem{
	public:
		virtual void Start() override;
		virtual void Run() override;
		virtual void Clear() override;

		void SDF_Importer(const std::string& path);
		std::vector<SDFData> sdfData;
	};

}