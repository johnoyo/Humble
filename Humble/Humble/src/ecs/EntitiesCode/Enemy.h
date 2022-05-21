#pragma once
#include <iostream>
#include "../Header.h"
#include "../Utilities.h"

namespace Enemy {

	namespace Level_0 {

		void init(float dt) {
			std::cout << "Calling enemy init 0 \n";
			GET_COMPONENT(Material, enemy).texture = "res/textures/player_r.png";
			//GET_COMPONENT(Transform, enemy).rotation = 45.0f;

		}

		void update(float dt) {
			//std::cout << "Calling enemy update 0 \n";
			GET_COMPONENT(Transform, enemy).position.x--;
			//GET_COMPONENT(Transform, enemy).rotation++;
			GET_COMPONENT(Gravity, enemy).Enabled = false;
		}

	}

	namespace Level_1 {

		void init(float dt) {
			std::cout << "Calling enemy init 1 \n";
			GET_COMPONENT(Material, enemy).texture = "res/textures/player_r.png";
			//GET_COMPONENT(Transform, enemy).rotation = 45.0f;
		}

		void update(float dt) {
			//std::cout << "Calling enemy update 1 \n";
			GET_COMPONENT(Transform, enemy).position.x++;
			GET_COMPONENT(Gravity, enemy).Enabled = true;;
		}

	}

}
