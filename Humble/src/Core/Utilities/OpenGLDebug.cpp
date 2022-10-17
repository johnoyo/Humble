#include "OpenGLDebug.h"

static HBL::GLDebug::DebugLogLevel s_DebugLogLevel = HBL::GLDebug::DebugLogLevel::HighAssert;

void HBL::GLDebug::SetGLDebugLogLevel(DebugLogLevel level)
{
	s_DebugLogLevel = level;
}

void HBL::GLDebug::OpenGLLogMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:
		if ((int)s_DebugLogLevel > 0)
		{
			ENGINE_LOG("[OpenGL Debug HIGH] {0}", message);
			if (s_DebugLogLevel == DebugLogLevel::HighAssert)
				ASSERT(false, "GL_DEBUG_SEVERITY_HIGH");
		}
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		if ((int)s_DebugLogLevel > 2)
			ENGINE_LOG("[OpenGL Debug MEDIUM] {0}", message);
		break;
	case GL_DEBUG_SEVERITY_LOW:
		if ((int)s_DebugLogLevel > 3)
			ENGINE_LOG("[OpenGL Debug LOW] {0}", message);
		break;
	case GL_DEBUG_SEVERITY_NOTIFICATION:
		if ((int)s_DebugLogLevel > 4)
			ENGINE_LOG("[OpenGL Debug NOTIFICATION] {0}", message);
		break;
	}
}

void HBL::GLDebug::EnableGLDebugging()
{
	glDebugMessageCallback(OpenGLLogMessage, nullptr);
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
}
