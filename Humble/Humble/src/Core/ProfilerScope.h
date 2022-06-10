#pragma once
#include <string>
#include <chrono>
#include <iostream>

namespace HBL {

	class ProfilerScope {
	public:
		std::chrono::time_point<std::chrono::system_clock> _timeStart;
		std::string _name;

		ProfilerScope(std::string name) {
			_timeStart = std::chrono::system_clock::now();
			_name = name;
		}

		ProfilerScope() {
			_timeStart = std::chrono::system_clock::now();
			_name = "scope";
		}

		~ProfilerScope() {
			auto _timeEnd = std::chrono::system_clock::now();

			std::chrono::duration<double> elapsed_seconds = _timeEnd - _timeStart;

			std::cout << "ENGINE PROFILE[" << __TIME__ << "]: Elapsed time of " << _name << " : " << elapsed_seconds.count() * 1000.0 << "ms" << std::endl;
		}
	};

}
