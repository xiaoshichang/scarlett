#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

#include <iostream>
using namespace std;

int main() {
	FT_Library  library;   /* handle to library     */
	FT_Face     face;      /* handle to face object */
	auto error = FT_Init_FreeType(&library);
	if (error) { return 1; }

	error = FT_New_Face(library, "Asset/Fonts/arial.ttf", 0, &face);
	if (error == FT_Err_Unknown_File_Format)
	{
		return 1;
	}
	else if (error)
	{
		return 1;
	}

	cout << face->num_glyphs << endl;
	cout << face->face_flags << endl;

	return 0;
}