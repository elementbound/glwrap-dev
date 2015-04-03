#include "meshutil.h"
#include "glwrap/util.h"

#include <sstream>
#include <string>
//strtod because std::stod is broken on my compiler ( mingw 4.8.1 )
#include <cstdlib>
#include <fstream> 

#include <vector>
#include <set> 
#include <array>

#include <glm/glm.hpp>

namespace meshutil
{
	void read_vec2(std::istream& is, glm::vec2& v)
	{
		std::string component[2];
		is >> component[0] >> component[1];
		
		v.x = (float)atof(component[0].c_str());
		v.y = (float)atof(component[1].c_str());
	}

	void read_vec3(std::istream& is, glm::vec3& v)
	{
		std::string component[3];
		is >> component[0] >> component[1] >> component[2];
		
		v.x = (float)atof(component[0].c_str());
		v.y = (float)atof(component[1].c_str());
		v.z = (float)atof(component[2].c_str());
	}

	void load_obj(std::istream& is, separated_mesh& result_mesh)
	{
		typedef unsigned int index_t;
		
		result_mesh.clear_streams();
		
		std::string line;
		std::stringstream parser;
		std::string token; 
		
		std::vector<glm::vec3> obj_positions;
		std::vector<glm::vec3> obj_normals;
		std::vector<glm::vec2> obj_texcoords;
		std::vector<std::array<index_t, 3>> obj_vertices;
		std::set<std::array<index_t, 3>> vertex_combinations;
		
		//
		
		obj_positions.reserve(8192);
		obj_normals.reserve(8192);
		obj_texcoords.reserve(8192);
		
		//
		
		result_mesh.draw_mode = GL_TRIANGLES;
		result_mesh.storage_policy = GL_STATIC_DRAW;
		
		unsigned pos = result_mesh.add_stream();
		unsigned nor = result_mesh.add_stream();
		unsigned tex = result_mesh.add_stream();
		unsigned ind = result_mesh.add_stream();
		
		result_mesh[pos].type = GL_FLOAT;
		result_mesh[pos].buffer_type = GL_ARRAY_BUFFER;
		result_mesh[pos].components = 3;
		result_mesh[pos].normalized = 0;
		result_mesh[pos].name = "vertexPosition";
		
		result_mesh[nor].type = GL_FLOAT;
		result_mesh[nor].buffer_type = GL_ARRAY_BUFFER;
		result_mesh[nor].components = 3;
		result_mesh[nor].normalized = 0;
		result_mesh[nor].name = "vertexNormal";
		
		result_mesh[tex].type = GL_FLOAT;
		result_mesh[tex].buffer_type = GL_ARRAY_BUFFER;
		result_mesh[tex].components = 3;
		result_mesh[tex].normalized = 0;
		result_mesh[tex].name = "vertexTexcoord";
		
		result_mesh[ind].type = GL_UNSIGNED_INT;
		result_mesh[ind].buffer_type = GL_ELEMENT_ARRAY_BUFFER;
		result_mesh[ind].components = 1;
		result_mesh[ind].normalized = 0;
		result_mesh[ind].name = "";
		
		//
		
		while(is.good())
		{
			std::getline(is, line);
			
			//Ignore empty lines
			if(line.size() == 0)
				continue;
			
			//Ignore comments
			if(line[0] == '#')
				continue;
			
			parser.str(line);
			parser.seekg(0);
			
			parser >> token;
			
			if(token == "v")
			{
				glm::vec3 v;
				read_vec3(parser, v);
				
				obj_positions.push_back(v);
			} 
			else if(token == "vn")
			{
				glm::vec3 n;
				read_vec3(parser, n);
				
				obj_normals.push_back(n);
			}
			else if(token == "vt")
			{
				glm::vec2 uv;
				read_vec2(parser, uv);
				
				obj_texcoords.push_back(uv);
			}
			else if(token == "f")
			{
				std::array<index_t, 3> f;
				
				for(unsigned j=0; j<3; j++)
				{
					for(unsigned i=0; i<3; i++)
					{
						parser >> f[i];
						
						if(i!=2)
							parser.ignore(std::numeric_limits<std::streamsize>::max(), '/');
					}
					
					obj_vertices.push_back(f);
					vertex_combinations.insert(f);
				}
			}
		}
		
		for(const auto& f: vertex_combinations)
		{
			const index_t pos_index = f[0] - 1;
			const index_t uv_index = f[1] - 1;
			const index_t normal_index = f[2] - 1;
			
			result_mesh[pos].data << obj_positions[pos_index];
			result_mesh[tex].data << obj_texcoords[uv_index];
			result_mesh[nor].data << obj_normals[normal_index];
		}
		
		for(const auto& f: obj_vertices)
		{
			index_t index = std::distance(vertex_combinations.begin(), vertex_combinations.find(f));
			result_mesh[ind].data << (unsigned int)index;
		}
		
		result_mesh.upload();
	}
	
	void load_obj(const char* file, separated_mesh& result_mesh)
	{
		std::ifstream fis(file);
		load_obj(fis, result_mesh);
	}
};