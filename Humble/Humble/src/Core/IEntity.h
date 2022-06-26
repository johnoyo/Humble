#pragma once

#include <unordered_map>
#include <string>

namespace HBL {

	struct IEntity{
		int ID = -1;
		std::unordered_map<std::string, int> components;
	};

}