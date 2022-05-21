#pragma once
#include "../Header.h"
#include "InputSystem.h"

class GravitySystem {
public:
	void Start(float gravityForce, float thres);
	void ResetGravity(float gravityForce, float thres);
	void Run();
private:
	float force;
	float threshold;
};