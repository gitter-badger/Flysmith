#include "PCH.h"
#include "Font.h"
#include <ft2build.h>
#include "FileSystem.h"
#include FT_FREETYPE_H


void cuc::Font::LoadFromFile(const char* path)
{
	FileSystem fs;
	char fullPath[MAX_PATH];
	strcpy(fullPath, fs.GetFontsPath());
	strcat(fullPath, path);

	FT_Library library;
	auto ftError = FT_Init_FreeType(&library);
	assert(!ftError);

	FT_Face face;
	ftError = FT_New_Face(library, fullPath, 0, &face);
	assert(!ftError);

	ftError = FT_Set_Pixel_Sizes(face, 0, 48);
	assert(!ftError);

	for (FT_ULong c = 0; c < 128; c++)
	{
		ftError = FT_Load_Char(face, c, FT_LOAD_RENDER);
		assert(!ftError);

		auto bmp = face->glyph->bitmap;
		// TODO: Cache bitmap data
	}

	FT_Done_Face(face);
	FT_Done_FreeType(library);
}