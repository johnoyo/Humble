#pragma once

#include "../../Renderer.h"
#include "../Header.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include "stb_image/stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

struct ShaderProgramSource {
	std::string VertexSource;
	std::string FragmentSource;
};

/*Vertex buffer declaration*/
class VertexBuffer {
public:

	void Initialize(int total_buffer_size);

	void Update_Position_On_Quad(unsigned int indx, Component::Transform tr);
	void Update_Position_On_Quad(unsigned int indx, glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, glm::vec2 p3);
	void Update_PositionX_On_Quad(unsigned int indx, Component::Transform tr);
	void Update_PositionY_On_Quad(unsigned int indx, Component::Transform tr);
	void Update_Material_On_Quad(unsigned int indx, glm::vec4 color, float tex_id);
	void Update_Material_On_Quad(unsigned int indx, glm::vec4 color, float tex_id, glm::vec2 coords, glm::vec2 sheet_size, glm::vec2 sp_size);


	void Set_Buffer(struct Vertex_Array* new_buffer);
	struct Vertex_Array* Get_Buffer();
	unsigned int Get_Size();
	void Set_Size(unsigned int size);
	unsigned int Get_Total_Size();
	void Reset();
	void Clean();

	unsigned int total_size = 0;
	friend class RenderingSystem;

private:
	struct Vertex_Array* buffer = NULL;
	unsigned int index = 0;
	void Fill_Buffer(glm::vec2 position, glm::vec4 color, glm::vec2 tex_coords, float tex_id);
};

/*IndexBuffer declaration*/
class IndexBuffer {
public:
	void Make_Indecies(unsigned int size);
	void Clean();
	unsigned int* Get_Index_Buffer();

private:
	unsigned int* index_buffer;
	unsigned int index = 0;
};


/*RenderingSystem declaration*/
class RenderingSystem {
public:

	void Start(glm::mat4 vpMatrix);
	void Run(int playerTransformID);
	void Render(glm::mat4 m_Camera_vp);
	void Clear();
	inline VertexBuffer& Get_Vertex_Buffer() { return vbuffer; }
	inline IndexBuffer& Get_Index_Buffer() { return ibuffer; }

	friend class LevelSystem;

	void Draw_Quad(glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec4 color);
	void Draw_Quad(glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, glm::vec2 p3);
	void Draw_Quad(int index);
	void Invalidate();

private:
	VertexBuffer vbuffer;
	IndexBuffer ibuffer;
	ShaderProgramSource sp;

	unsigned int vao;
	unsigned int vb;
	unsigned int ib;
	unsigned int shader;

	ShaderProgramSource ParseShader(const std::string& filepath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

	void Initialize(glm::mat4 m_Camera_vp);
	void Upadte_Index_Buffer(unsigned int size);
	void Init_Vertex_Buffer();
	void Update_Vertex_Buffer_Positions(int playerTransformID);
	void Update_Camera_Uniform(glm::mat4 m_Camera_vp);
};


