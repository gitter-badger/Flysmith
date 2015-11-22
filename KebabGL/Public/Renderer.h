#pragma once
#include "PublicDef.h"
#include "PCH.h"


class KEBABGL_API Renderer
{
public:
	Renderer(const HWND windowHandle, const U32 windowWidth, const U32 windowHeight);
	~Renderer();
	void Render();

private:
	struct Impl;
	Impl* m_pImpl;
};