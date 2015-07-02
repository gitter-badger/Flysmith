#include "GLContext.h"
#include "GL\glew.h"
#include "GL\wglew.h"
using namespace cuc;


GLContext::GLContext(HWND hWindow, U32 width, U32 height)
	: m_hWindow(hWindow)
	, m_hDeviceContext(0)
	, m_hRenderContext(0)
{
	m_hDeviceContext = GetDC(hWindow);

	CreatePixelFormat();
	CreateContext();

	// Set clear color
	glClearColor(0.4f, 0.6f, 0.9f, 0.0f);

	glViewport(0, 0, width, height);
}

GLContext::~GLContext()
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
	// Create a temporary Opengl 2.1 context to allow the initialization of GLEW
	auto tempContext = wglCreateContext(m_hDeviceContext);
	wglMakeCurrent(m_hDeviceContext, tempContext);

	glewInit();

	// If the context creation extension is available, create an Opengl 4.3 context and make it active(replacing the temporary one)
	// If not, keep using the 2.1 context
	if (wglewIsSupported("WGL_ARB_create_contxt"))
	{
		int attributes[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
			WGL_CONTEXT_MINOR_VERSION_ARB, 3,
			WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
			0
		};

		m_hRenderContext = wglCreateContextAttribsARB(m_hDeviceContext, nullptr, attributes);
		
		wglDeleteContext(tempContext);
		wglMakeCurrent(m_hDeviceContext, m_hRenderContext);
	}
	else
	{
		m_hRenderContext = tempContext;
	}
}

void GLContext::SwapContextBuffers()
{
	SwapBuffers(m_hDeviceContext);
}

void GLContext::Resize(U32 width, U32 height)
{
	glViewport(0, 0, width, height);
}