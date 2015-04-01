#include "texture_util.h"

namespace texutil
{
	texture2d load_png(const char* file) { return load_png(png::image<png::rgba_pixel>(file)); }
	
	texture2d load_png(const png::image<png::rgba_pixel>& png)
	{
		texture2d ret;
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
		
		ret.upload(data, png.get_width(),png.get_height(), GL_RGBA,GL_RGBA, GL_UNSIGNED_BYTE);
		delete [] data;
		
		return ret;
	}
};