#pragma once

namespace HBL {

	class ScriptFunction {
	public:
		ScriptFunction() {}
		virtual ~ScriptFunction() {}

		virtual void Init() = 0;
		virtual void Update() = 0;
	};

}