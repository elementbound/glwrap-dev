#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glwrap/window.h"
#include "glwrap/shader.h"
#include "glwrap/mesh.h"
#include "glwrap/util.h"

#include <iostream>
#include <string>

void error_callback(int error, const char* error_str)
{
	std::cerr << "[" << error << "]" << error_str << std::endl;
}

class window_texture: public window
{
	private:
		shader_program program;
		separated_mesh mesh;
		GLuint texture;
		
	protected: 
		void on_open()
		{
			this->make_current();
			
			if(glewInit() != GLEW_OK)
			{
				std::cerr << "GLEW init fail" << std::endl;
				return;
			}
		
			if(!GLEW_VERSION_3_3)
			{
				std::cerr << "OpenGL 3.3 not supported" << std::endl;
				return; 
			}
			
			std::cout << "Creating mesh... ";
			{
				mesh.draw_mode = GL_TRIANGLE_FAN;
				mesh.storage_policy = GL_STATIC_DRAW;
				
				unsigned pos = mesh.add_stream();
				unsigned uvs = mesh.add_stream();
				
				mesh[pos].type = GL_FLOAT;
				mesh[pos].buffer_type = GL_ARRAY_BUFFER;
				mesh[pos].components = 2;
				mesh[pos].normalized = 0;
				mesh[pos].name = "vertexPosition";
				
				mesh[pos].data <<
					0.0f << 0.0f <<
					1.0f << 0.0f << 
					1.0f << 1.0f << 
					0.0f << 1.0f;
					
				//
				
				mesh[uvs].type = GL_FLOAT;
				mesh[uvs].buffer_type = GL_ARRAY_BUFFER;
				mesh[uvs].components = 2;
				mesh[uvs].normalized = 0;
				mesh[uvs].name = "vertexTexcoord";
				
				mesh[uvs].data <<
					0.0f << 0.0f <<
					1.0f << 0.0f << 
					1.0f << 1.0f << 
					0.0f << 1.0f;
				
				mesh.upload();
			}
			std::cout << "Done" << std::endl;
			
			std::cout << "Creating textures... ";
			{
				glGenTextures(1, &texture);
				float tex_rgb[] = {
					0.5f, 0.5f, 0.5f, 
					1.0f, 1.0f, 1.0f, 
					1.0f, 1.0f, 1.0f,
					0.5f, 0.5f, 0.5f
				};
				
				glBindTexture(GL_TEXTURE_2D, texture);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2,2,0, GL_RGB, GL_FLOAT, tex_rgb);
				
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			}
			
			//Shaders
			std::cout << "Compiling shaders... ";
			program.create();
			
			if(!program.attach(read_file("data/textured.vs").c_str(), shader_program::shader_type::vertex))
			{
				std::cerr << "Couldn't attach vertex shader" << std::endl;
				return;
			}
			
			if(!program.attach(read_file("data/textured.fs").c_str(), shader_program::shader_type::fragment))
			{
				std::cerr << "Couldn't attach fragment shader" << std::endl;
				return;
			}
			
			glBindFragDataLocation(program.handle(), 0, "outColor");
			program.link();
			program.use();
			
			//Link attribs
			mesh.bind();
			
			std::cout << "Ready to use" << std::endl;
		}
		
		void on_refresh()
		{
			static float f = 0.0;
			f += 1.0/256.0;
			if(f >= 1.0) f -= 2.0f;
			
			glClear(GL_COLOR_BUFFER_BIT);
			
			static glm::mat4 matView(1.0);
			program.set_uniform("uMVP", matView);
			mesh.draw();
			
			glfwSwapBuffers(this->handle());
			
			if(glfwGetKey(this->handle(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
				glfwSetWindowShouldClose(this->handle(), 1);
		}
};

int main()
{
	glfwSetErrorCallback(error_callback);
	if(!glfwInit())
		return 1;
		
	window_texture wnd;
	
	glewExperimental = GL_TRUE;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	wnd.open(512,512, "it's a tRIANGLE");
	if(!wnd)
		return 3;
	
	wnd.make_current();
	
	std::cout << "Init done, starting loop" << std::endl;
	
	while(!glfwWindowShouldClose(wnd.handle()))
	{
		wnd.refresh();
		glfwPollEvents();
	}
	
	return 0;
}