#include "texture_util.h"

namespace texutil
{
	image load_png(const char* file) { return load_png(png::image<png::rgba_pixel>(file)); }
	
	image load_png(const png::image<png::rgba_pixel>& png)
	{
		image ret;
		if(png.get_width()==0 || png.get_height()==0)
			return ret;
		
		unsigned char* data = new unsigned char[png.get_width() * png.get_height() * 4];
		for(unsigned y=0; y<png.get_height(); y++)
			for(unsigned x=0; x<png.get_width(); x++)
			{
				png::rgba_pixel p = png.get_pixel(x, png.get_height() - (y+1));
				data[(y*png.get_width() + x)*4 + 0] = p.red;
				data[(y*png.get_width() + x)*4 + 1] = p.green;
				data[(y*png.get_width() + x)*4 + 2] = p.blue;
				data[(y*png.get_width() + x)*4 + 3] = p.alpha;
			}
		
		ret.data = (char*)data;
		ret.size = png.get_width() * png.get_height() * 4;
		
		ret.width = png.get_width();
		ret.height = png.get_height();
		
		ret.data_format = GL_RGBA;
		ret.data_type = GL_UNSIGNED_BYTE;
		
		return ret;
	}
};