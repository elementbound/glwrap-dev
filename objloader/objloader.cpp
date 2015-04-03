#include <iostream>
#include <sstream>
#include <string>
//strtod because std::stod is broken on my compiler ( mingw 4.8.1 )
#include <cstdlib> 
#include <vector>
#include <set> 
#include <array>

#include <glm/glm.hpp>
#include <glm/gtx/io.hpp>

void read_vec2(std::istream& is, glm::dvec2& v)
{
	std::string component[2];
	is >> component[0] >> component[1];
	
	v.x = atof(component[0].c_str());
	v.y = atof(component[1].c_str());
}

void read_vec3(std::istream& is, glm::dvec3& v)
{
	std::string component[3];
	is >> component[0] >> component[1] >> component[2];
	
	v.x = atof(component[0].c_str());
	v.y = atof(component[1].c_str());
	v.z = atof(component[2].c_str());
}

typedef int index_t;

int main()
{
	std::string line;
	std::stringstream parser;
	std::string token; 
	
	std::vector<glm::dvec3> obj_positions;
	std::vector<glm::dvec3> obj_normals;
	std::vector<glm::dvec2> obj_texcoords;
	std::vector<std::array<index_t, 3>> obj_vertices;
	std::set<std::array<index_t, 3>> vertex_combinations;
	
	std::vector<glm::dvec3> vertex_positions;
	std::vector<glm::dvec3> vertex_normals;
	std::vector<glm::dvec2> vertex_texcoords;
	std::vector<index_t> element_buffer;
	
	obj_positions.reserve(8192);
	obj_normals.reserve(8192);
	obj_texcoords.reserve(8192);
	
	vertex_positions.reserve(8192);
	vertex_normals.reserve(8192);
	vertex_texcoords.reserve(8192);
	element_buffer.reserve(8192);
	
	std::cout << "Parsing file... ";
	while(std::cin.good())
	{
		std::getline(std::cin, line);
		
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
			glm::dvec3 v;
			read_vec3(parser, v);
			
			obj_positions.push_back(v);
		} 
		else if(token == "vn")
		{
			glm::dvec3 n;
			read_vec3(parser, n);
			
			obj_normals.push_back(n);
		}
		else if(token == "vt")
		{
			glm::dvec2 uv;
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
		else 
			std::cout << "Unknown token: " << token << '\n';
	}
	std::cout << "Done\n";
	
	std::cout << "Filling buffers... ";
	for(const auto& f: vertex_combinations)
	{
		const index_t& pos_index = f[0];
		const index_t& uv_index = f[1];
		const index_t& normal_index = f[2];
		
		vertex_positions.push_back(obj_positions[pos_index-1]);
		vertex_texcoords.push_back(obj_texcoords[uv_index-1]);
		vertex_normals.push_back(obj_normals[normal_index-1]);
	}
	std::cout << "Done\n";
	
	std::cout << "Writing indices... ";
	for(const auto& f: obj_vertices)
	{
		index_t index = std::distance(vertex_combinations.begin(), vertex_combinations.find(f));
		element_buffer.push_back(index);
	}
	std::cout << "Done\n";
	
	std::cout << "Positions: " << obj_positions.size() << '\n'
			  << "Normals:   " << obj_normals.size() << '\n'
			  << "Texcoords: " << obj_texcoords.size() << '\n'
			  << "Vertices:  " << obj_vertices.size() << '\n'
			  << "Combinations: " << vertex_combinations.size() << '\n';
	
	return 0;
}