#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include "glwrap/window.h"
#include "glwrap/resizable_window.h"
#include "glwrap/shader.h"
#include "glwrap/mesh.h"
#include "glwrap/util.h"

#include "meshutil.h"

#include <iostream>
#include <string>

void error_callback(int error, const char* error_str)
{
	std::cerr << "[" << error << "]" << error_str << std::endl;
}

class window_objloader: public resizable_window
{
	private:
		shader_program program;
		separated_mesh mesh;
		
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
			
			//GL init
			glEnable(GL_DEPTH_TEST);
			
			//Asset init
			std::cout << "Loading mesh... ";
			meshutil::load_obj("data/cube.obj", mesh);
			mesh.draw_mode = GL_TRIANGLES;
			std::cout << "Done" << std::endl;
			
			//Shaders
			std::cout << "Compiling shaders... ";
			program.create();
			
			if(!program.attach(read_file("data/simple2.vs").c_str(), shader_program::shader_type::vertex))
			{
				std::cerr << "Couldn't attach vertex shader" << std::endl;
				return;
			}
			
			if(!program.attach(read_file("data/simple2.fs").c_str(), shader_program::shader_type::fragment))
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
		
		void on_resize(int w, int h)
		{
			resizable_window::on_resize(w,h);
			matProjection = glm::perspective(glm::radians(45.0f), (float)m_WindowAspect, 0.5f, 16.0f);
		}
		
		void on_refresh()
		{
			static float r = 2.0f;
			static float pitch = glm::radians(45.0f);
			static glm::vec3 camera_at;
			
			static float f = 0.0;
			f = fmod(glfwGetTime()/8.0f, 1.0f);
			
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			float angle = f * 2.0 * 3.14159265;
			pitch = glm::radians(45.0f + std::sin(f*2.0f*glm::two_pi<float>())*11.25f);
			camera_at = r * dirvec(angle, pitch);
			
			matView = glm::lookAt(camera_at, glm::vec3(0.0f), glm::vec3(0.0f,0.0f,1.0f));
			
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
		
	window_objloader wnd;
	
	glewExperimental = GL_TRUE;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	wnd.open(512,512, "it's an objECT");
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