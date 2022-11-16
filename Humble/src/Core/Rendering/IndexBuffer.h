#pragma once

#include "../Core.h"
#include "../HumbleAPI.h"

namespace HBL 
{
	class HBL_API IndexBuffer 
	{
	public:
		void MakeIndecies(uint32_t size);
		void Clean();
		uint32_t* GetIndexBuffer();

	private:
		uint32_t* m_IndexBuffer = nullptr;
		uint32_t m_Index = 0;
	};
}