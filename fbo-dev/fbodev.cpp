#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>

#include "fbodev.h"

#include <iostream>
#include <string>

//===========================================================================================//
//Window implementation

#define die(msg) {std::cerr << msg << std::endl; std::exit(1);}
#define dieret(msg, val) {std::cerr << msg << std::endl; return val;}

//Not strictly for existence, more like for accessibility
//Which I basically need this for, so move on... 
bool file_exists(const char* fname)
{
	std::ifstream fs(fname);
	return fs.good();
}

bool app_FboDev::init_glew()
{
	int err;
	if((err=glewInit()) != GLEW_OK)
		dieret("GLEW init: " << glewGetErrorString(err), 0);

	if(!GLEW_VERSION_3_3)
		dieret("OpenGL 3.3 not supported", 0);
	
	return 1;
}

void app_FboDev::init_window()
{
	int w, h;
	glfwGetFramebufferSize(this->handle(), &w, &h);
	this->on_fbresize(w,h);
	
	glfwSetInputMode(this->handle(), GLFW_STICKY_MOUSE_BUTTONS, GL_TRUE);
	
	//GL init
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	
	m_View = glm::mat4(1.0f);
}

bool app_FboDev::load_resources()
{
	std::vector<const char*> file_list = 
	{
		"data/diffuse.vs",
		"data/diffuse.fs",

		"data/wireframe.vs",
		"data/wireframe.fs"
	};
	
	bool fail = false;
	for(const char* fname : file_list) 
		if(!file_exists(fname))
		{
			std::cerr << '\"' << fname << "\" missing!\n";
			fail = true;
		}
		
	if(fail) 
		return 0;
	
	std::cout << "Compiling diffuse shader... ";
		m_DiffuseShader.create();
		
		if(!m_DiffuseShader.attach(read_file("data/diffuse.vs").c_str(), shader_program::shader_type::vertex))
			dieret("\nCouldn't attach vertex shader", 0);
		
		if(!m_DiffuseShader.attach(read_file("data/diffuse.fs").c_str(), shader_program::shader_type::fragment))
			dieret("\nCouldn't attach fragment shader", 0);
		
		glBindFragDataLocation(m_DiffuseShader.handle(), 0, "outColor");
		m_DiffuseShader.link();
	std::cout << "done\n";
	
	return 1;
}

void app_FboDev::on_open()
{
	this->make_current();
	
	if(!init_glew())
		die("GLEW init fail!");
	
	init_window();
	
	if(!load_resources())
		die("Couldn't load resources!");
}

void app_FboDev::on_fbresize(int w, int h)
{
	resizable_window::on_fbresize(w,h);
	
	w += (w==0);
	h += (h==0);
	
	m_Projection = glm::perspective(m_CameraFov, float(w)/h, 1e-2f, 1024.0f);
	m_Viewport = glm::vec4(0.0f, 0.0f, (float)m_FramebufferWidth, (float)m_FramebufferHeight);
}

void app_FboDev::on_key(int key, int scancode, int action, int mods)
{
	if(key == GLFW_KEY_ESCAPE)
		glfwSetWindowShouldClose(this->handle(), 1);
}

void app_FboDev::on_refresh()
{
	//Draw
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	m_CameraAt = dirvec(m_CameraRot.y, m_CameraRot.x) * m_CameraDst;
	m_View = glm::lookAt(m_CameraAt, glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,0.0f,1.0f));

	if(m_DrawMode & 0x1) {
		glEnable(GL_DEPTH_TEST);
			m_DiffuseShader.use();
			m_DiffuseShader.set_uniform("uModelView", m_View); 
			m_DiffuseShader.set_uniform("uProjection", m_Projection); 
			m_DiffuseShader.set_uniform("uLightDir", glm::vec3(0.707f, 0.707f, 0.0f)); 
			m_Mesh.draw();
	}

	if(m_DrawMode & 0x2) {
		glDisable(GL_DEPTH_TEST);
			m_WireShader.use();
			m_WireShader.set_uniform("uMVP", m_Projection * m_View); 
			m_WireShader.set_uniform("uColor", glm::vec4(0,0,0, 0.125f));
			m_Wireframe.draw();
	}

	if(m_DrawMode & 0x4) {
		glEnable(GL_DEPTH_TEST);
			m_WireShader.use();
			m_WireShader.set_uniform("uMVP", m_Projection * m_View); 
			m_WireShader.set_uniform("uColor", glm::vec4(0,0,0, 0.5f));
			m_Wireframe.draw();
	}
	
	glfwSwapBuffers(this->handle());
}

//===========================================================================================//
//Runner

void error_callback(int error, const char* error_str)
{
	std::cerr << "[" << error << "]" << error_str << std::endl;
}

int main()
{
	glfwSetErrorCallback(error_callback);
	if(!glfwInit())
		return 1;
		
	app_FboDev wnd;
	
	glewExperimental = GL_TRUE;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	wnd.open(512,512, "fEED. bUFFER. oCCLUDE.");
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