#pragma once
#include "../../Renderer.h"
#include "../Header.h"
#include "RenderingSystem.h"
#include <vector>
#include <iostream>
#include "stb_image/stb_image.h"

/*Texture Manager declararion*/
class TextureSystem {
public:
	void Start();
	void Run(RenderingSystem &rend);
	void Clear();

private:
	unsigned int texture_slot[32];

	unsigned int current_index = 0;
	std::vector<std::string> map;
	std::vector<glm::vec2> size;
	void Init_Transparent_Texture();
	void Load_Texture(const std::string& path);
	float Find(const std::string& path);


};