#pragma once

namespace HBL {

	class Scene {
	public:
		virtual void Enroll() = 0;
		virtual void Add() = 0;
		virtual void Init() = 0;
		virtual void Update() = 0;
		void Set_Level(const std::string& level) { _level = level; }
		std::string& Get_Level() { return _level; }
	private:
		std::string _level;
	};

}
