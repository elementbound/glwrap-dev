#ifndef _H_TEXTURE_UTIL_H_
#define _H_TEXTURE_UTIL_H_ 

#include "texture.h"
#include <cstring> //png++ requires strerror
#include <png++/png.hpp>

namespace texutil
{
	image load_png(const char* file);
	image load_png(const png::image<png::rgba_pixel>& png);
};

#endif