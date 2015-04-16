#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glwrap/window.h"
#include "glwrap/resizable_window.h"
#include "glwrap/shader.h"
#include "glwrap/mesh.h"
#include "glwrap/meshutil.h"
#include "glwrap/util.h"
#include "glwrap/texture.h"
#include "glwrap/texture_util.h"

#include <iostream>
#include <string>

void error_callback(int error, const char* error_str)
{
	std::cerr << "[" << error << "]" << error_str << std::endl;
}

class window_texture: public resizable_window
{
	private:
		shader_program program;
		separated_mesh mesh;
		texture_cube tex_skybox;
		
		glm::mat4 matView;
		glm::mat4 matProjection;
		
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
				meshutil::load_obj("data/cube.obj", mesh);
				//mesh.draw_mode = GL_TRIANGLES;
				mesh.storage_policy = GL_STATIC_DRAW;
				mesh.upload();
			}
			std::cout << "Done" << std::endl;
			
			glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
			
			//Textures
			std::cout << "Creating textures... ";
			{
				tex_skybox.upload(GL_TEXTURE_CUBE_MAP_POSITIVE_X, texutil::load_png("data/skybox/alps_lf.png"), GL_RGB);
				tex_skybox.upload(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, texutil::load_png("data/skybox/alps_rt.png"), GL_RGB);
				tex_skybox.upload(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, texutil::load_png("data/skybox/alps_bk.png"), GL_RGB);
				tex_skybox.upload(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, texutil::load_png("data/skybox/alps_ft.png"), GL_RGB);
				tex_skybox.upload(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, texutil::load_png("data/skybox/alps_dn.png"), GL_RGB);
				tex_skybox.upload(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, texutil::load_png("data/skybox/alps_up.png"), GL_RGB);
				
				tex_skybox.parameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				tex_skybox.parameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				
				tex_skybox.generate_mipmaps();
				
				tex_skybox.use();
			}
			std::cout << "Done" << std::endl;
			
			//Shaders
			std::cout << "Compiling shaders... ";
			program.create();
			
			if(!program.attach(read_file("data/skybox.vs").c_str(), shader_program::shader_type::vertex))
			{
				std::cerr << "Couldn't attach vertex shader" << std::endl;
				return;
			}
			
			if(!program.attach(read_file("data/skybox.fs").c_str(), shader_program::shader_type::fragment))
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
			
			//
			
			{
				int w, h;
				glfwGetWindowSize(this->handle(), &w, &h);
				this->on_resize(w,h);
			}
		}
		
		void on_resize(int w, int h)
		{
			resizable_window::on_resize(w,h);
			matProjection = glm::perspective(glm::radians(80.0f), (float)m_WindowAspect, 0.5f, 16.0f);
		}
		
		void on_refresh()
		{
			static float r = 4.0f;
			static float pitch = glm::radians(45.0f);
			static glm::vec3 camera_at;
			
			static float f = 0.0;
			f = fmod(glfwGetTime()/30.0f, 1.0f);
			
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			float angle = f * 2.0 * 3.14159265;
			pitch = glm::radians(std::sin(f*2.0f*glm::two_pi<float>())*60.0f);
			camera_at = r * dirvec(angle, pitch);
			
			matView = glm::lookAt(glm::vec3(0.0f), camera_at, glm::vec3(0.0f,0.0f,1.0f));
			
			program.set_uniform("uMVP", matProjection * matView);
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
	
	wnd.open(512,512, "demSKY");
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