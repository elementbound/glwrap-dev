#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>

#include "fbodev.h"
#include "glwrap/meshutil.h"

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
		"data/textured.vs",
		"data/textured.fs",

		"data/post.vs",
		"data/post.fs",

		"data/cube.obj"
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
	
	std::cout << "Compiling texture shader... ";
		m_TexturedShader.create();
		
		if(!m_TexturedShader.attach(read_file("data/textured.vs").c_str(), shader_program::shader_type::vertex))
			dieret("\nCouldn't attach vertex shader", 0);
		
		if(!m_TexturedShader.attach(read_file("data/textured.fs").c_str(), shader_program::shader_type::fragment))
			dieret("\nCouldn't attach fragment shader", 0);
		
		glBindFragDataLocation(m_TexturedShader.handle(), 0, "outColor");
		m_TexturedShader.link();
	std::cout << "done\n";

	//

	std::cout << "Loading cube mesh... ";
		load_obj("data/cube.obj", m_CubeMesh);

		m_TexturedShader.use();
		m_CubeMesh.bind();
	std::cout << "done\n"
	
	std::cout << "Creating screen plane mesh... ";
	{
		unsigned pos = m_ScreenPlaneMesh.add_stream();
		unsigned tex = m_ScreenPlaneMesh.add_stream();

		m_ScreenPlaneMesh[pos].type = GL_FLOAT;
		m_ScreenPlaneMesh[pos].buffer_type = GL_ARRAY_BUFFER;
		m_ScreenPlaneMesh[pos].components = 2;
		m_ScreenPlaneMesh[pos].normalized = 0;
		m_ScreenPlaneMesh[pos].name = "vertexPosition";

		m_ScreenPlaneMesh[pos].data << 
			-1.0f << -1.0f <<
			 1.0f << -1.0f << 
			 1.0f <<  1.0f << 

			-1.0f << -1.0f << 
			 1.0f <<  1.0f << 
			-1.0f <<  1.0f;

		m_ScreenPlaneMesh[tex].type = GL_FLOAT;
		m_ScreenPlaneMesh[tex].buffer_type = GL_ARRAY_BUFFER;
		m_ScreenPlaneMesh[tex].components = 2;
		m_ScreenPlaneMesh[tex].normalized = 0;
		m_ScreenPlaneMesh[tex].name = "vertexTexcoord";

		m_ScreenPlaneMesh[tex].data << 
			0.0f << 0.0f <<
			1.0f << 0.0f << 
			1.0f << 1.0f << 

			0.0f << 0.0f << 
			1.0f << 1.0f << 
			0.0f << 1.0f;

		m_ScreenPlaneMesh.upload();

		m_PostprocessShader.use();
		m_ScreenPlaneMesh.bind();
	}
	std::cout << "done\n";

	//

	std::cout << "Creating FBO... \n";
		std::cout << "\tCreating texture... ";
			glGenTextures(1, &m_TextureHandle);
			glBindTexture(GL_TEXTURE_2D, m_TextureHandle);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_FBOSize.x, m_FBOSize.y, 0, GL_UNSIGNED_BYTE, NULL);
		std::cout << "done!\n";

		std::cout << "\tCreating RBO... ";
			glGenRenderBuffers(1, &m_RBOHandle);
			glBindRenderBuffer(GL_RENDERBUFFER, m_RBOHandle);
			glRenderBufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_FBOSize.x, m_FBOSize.y);
		std::cout << "done\n";

		glGenFramebuffers(1, &m_FBOHandle);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBOHandle);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE2D, m_TextureHandle, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_RBOHandle);
	std::cout << "Done!\n";

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
	
	m_WindowProjection = glm::ortho(0.0f, (float)m_FramebufferWidth, 0.0f, (float)m_FramebufferHeight);
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