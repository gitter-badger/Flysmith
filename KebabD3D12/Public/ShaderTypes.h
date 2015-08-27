#pragma once


// Added _SHADER suffix because of a preprocessor define DOMAIN in math.h
enum ShaderType
{
	COMPUTE_SHADER,
	VERTEX_SHADER,
	HULL_SHADER,
	DOMAIN_SHADER,
	GEOMETRY_SHADER,
	PIXEL_SHADER
};