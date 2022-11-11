#pragma once
#include "../Core.h"
#include "../HumbleAPI.h"

namespace HBL {

	class HBL_API IndexBuffer 
	{
	public:
		void Make_Indecies(uint32_t size);
		void Clean();
		uint32_t* Get_Index_Buffer();

	private:
		uint32_t* m_IndexBuffer = nullptr;
		uint32_t m_Index = 0;
	};

}