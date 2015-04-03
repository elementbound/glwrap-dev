#ifndef _H_MESHUTIL_H_
#define _H_MESHUTIL_H_

#include <iostream>
#include "glwrap/mesh.h"

namespace meshutil
{
	void load_obj(const char* file, separated_mesh& result_mesh);
	void load_obj(std::istream& is, separated_mesh& result_mesh);
};

#endif