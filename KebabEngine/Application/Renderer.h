#pragma once
#include "KebabEngine.h"
#include "GL\glew.h"
#include "GL\wglew.h"
#include "GLContext.h"


namespace cuc
{
	class Renderer
	{
	public:
		GLContext* m_pContext;

		void Render()
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			m_pContext->SwapContextBuffers();
		}
	};
}