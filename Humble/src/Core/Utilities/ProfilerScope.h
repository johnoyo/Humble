#pragma once
#include <string>
#include <chrono>
#include <iostream>
#include "HumbleAPI.h"

namespace HBL 
{
	class HBL_API ProfilerScope 
	{
	public:
		std::chrono::time_point<std::chrono::system_clock> m_TimeStart = std::chrono::system_clock::now();
		std::string m_Name = "";

		ProfilerScope(const std::string& name) 
		{
			m_TimeStart = std::chrono::system_clock::now();
			m_Name = name;
		}

		ProfilerScope() 
		{
			m_TimeStart = std::chrono::system_clock::now();
			m_Name = "Scope";
		}

		~ProfilerScope() 
		{
			auto timeEnd = std::chrono::system_clock::now();

			std::chrono::duration<double> elapsed_seconds = timeEnd - m_TimeStart;

			std::cout << "ENGINE PROFILE[" << __TIME__ << "]: Elapsed time of " << m_Name << " : " << elapsed_seconds.count() * 1000.0 << "ms" << std::endl;
		}
	};
}
