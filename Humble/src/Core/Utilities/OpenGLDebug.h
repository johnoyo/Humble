#pragma once
#include "../Core.h"
#include "../Utilities.h"
#include "../HumbleAPI.h"

namespace HBL 
{
	namespace GLDebug 
	{
		enum class DebugLogLevel
		{
			None = 0, HighAssert = 1, High = 2, Medium = 3, Low = 4, Notification = 5
		};
	
		void EnableGLDebugging();
		void SetGLDebugLogLevel(DebugLogLevel level);
		void OpenGLLogMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
	}
}