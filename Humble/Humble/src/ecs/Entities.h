#pragma once
#include "Components.h"

namespace Entity {

	struct BaseEntity
	{
		int ID = -1;
		int Transform = -1;
		int CollisionBox = -1;
		int Material = -1;
		int Animation = -1;
		int Gravity = -1;
		int Script = -1;
		int Health = -1;
		int Shadow = -1;
	};

}

