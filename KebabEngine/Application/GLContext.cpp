#include "GLContext.h"
#include "GLCore.h"
#include "WGL.h"
using namespace cuc;


bool GLContext::s_bLoadedFunctions = false;

GLContext::GLContext(HWND hWindow, U32 width, U32 height)
	: m_hWindow(hWindow)
	, m_hDeviceContext(0)
	, m_hRenderContext(0)
{
	m_hDeviceContext = GetDC(hWindow);

	CreatePixelFormat();
	CreateContext();

	// Set clear color
	gl::ClearColor(0.4f, 0.6f, 0.9f, 0.0f);

	gl::Viewport(0, 0, width, height);
}

GLContext::~GLContext()
{
	DestroyContext();
}

void GLContext::DestroyContext()
{
	wglMakeCurrent(m_hDeviceContext, 0);
	wglDeleteContext(m_hRenderContext);
	ReleaseDC(m_hWindow, m_hDeviceContext);
}

void GLContext::CreatePixelFormat()
{
	PIXELFORMATDESCRIPTOR pixelFormatDesc;
	memset(&pixelFormatDesc, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pixelFormatDesc.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pixelFormatDesc.iPixelType = PFD_TYPE_RGBA;
	pixelFormatDesc.cColorBits = 32;
	pixelFormatDesc.cDepthBits = 32;
	pixelFormatDesc.iLayerType = PFD_MAIN_PLANE;

	auto pixelFormat = ChoosePixelFormat(m_hDeviceContext, &pixelFormatDesc);
	SetPixelFormat(m_hDeviceContext, pixelFormat, &pixelFormatDesc);
}

void GLContext::CreateContext()
{
	// Create a temporary Opengl 2.1 context to allow the loading of functions that can create a 4.5 context
	auto tempContext = wglCreateContext(m_hDeviceContext);
	wglMakeCurrent(m_hDeviceContext, tempContext);

	if (!s_bLoadedFunctions)
	{
		LoadFunctions();
	}
	
	if (!s_bLoadedFunctions)
	{
		// TODO: Decide if crash or allow using the 2.1 context
		// DestroyContext();
		m_hRenderContext = tempContext;
		return;
	}

	// If the context creation extension is available, create an Opengl 4.5 context and make it active(replacing the temporary one)
	// If not, keep using the 2.1 context
	if (wgl::exts::var_ARB_create_context)
	{
		int attributes[] = {
			wgl::CONTEXT_MAJOR_VERSION_ARB, 4,
			wgl::CONTEXT_MINOR_VERSION_ARB, 5,
			wgl::CONTEXT_FLAGS_ARB, wgl::CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
			0
		};

		m_hRenderContext = wgl::CreateContextAttribsARB(m_hDeviceContext, nullptr, attributes);

		wglDeleteContext(tempContext);
		wglMakeCurrent(m_hDeviceContext, m_hRenderContext);
	}
	else
	{
		// TODO: Decide if crash or allow using the 2.1 context
		// DestroyContext();
		m_hRenderContext = tempContext;
	}

}

void GLContext::LoadFunctions()
{
	auto glLoad = gl::sys::LoadFunctions();
	if (!glLoad)
	{
		// TODO: Log error
		return;
	}

	auto wglLoad = wgl::sys::LoadFunctions(m_hDeviceContext);
	if (!wglLoad)
	{
		// TODO: Log error
		return;
	}

	// TODO: Log number of functions that failed to load:
	//		 glLoad.GetNumMissing();
	//		 wglLoad.GetNumMissing();

	s_bLoadedFunctions = true;
}

void GLContext::SwapContextBuffers()
{
	SwapBuffers(m_hDeviceContext);
}

void GLContext::Resize(U32 width, U32 height)
{
	gl::Viewport(0, 0, width, height);
}