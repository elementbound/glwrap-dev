#ifndef _H_TEXTURE_UTIL_H_
#define _H_TEXTURE_UTIL_H_ 

#include "texture2d.h"
#include <cstring> //png++ requires strerror
#include <png++/png.hpp>

namespace texutil
{
	texture2d load_png(const char* file);
	texture2d load_png(const png::image<png::rgba_pixel>& png);
};

#endif