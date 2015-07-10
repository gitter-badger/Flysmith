#pragma once
#include "KebabEngine.h"
#include "GLContext.h"
#include "GLCore.h"


namespace cuc
{
	class Renderer
	{
	public:
		GLContext* m_pContext;

		void Render()
		{
			gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT | gl::STENCIL_BUFFER_BIT);
			m_pContext->SwapContextBuffers();
		}
	};
}