#ifndef _H_FBODEV_H_
#define _H_FBODEV_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "glwrap/window.h"
#include "glwrap/resizable_window.h"
#include "glwrap/shader.h"
#include "glwrap/mesh.h"

class app_FboDev : public resizable_window {
	private:
		shader				m_TexturedShader;
		shader 				m_PostprocessShader;

		separated_mesh 		m_CubeMesh;
		separated_mesh 		m_ScreenPlaneMesh;

		//

		GLuint				m_FBOHandle;
		GLuint				m_TextureHandle;
		GLuint				m_RBOHandle;

		//

		const glm::uvec2	m_FBOSize = {512, 512};
		glm::mat4			m_FBOView;
		glm::mat4			m_FBOProjection;

		glm::mat4			m_WindowProjection;

		//

		bool init_glew();
		void init_window();
		bool load_resources();

	protected: 
		void on_open();
		void on_fbresize(int w, int h);
		void on_key(int key, int scancode, int action, int mods);
		void on_refresh();
};

#endif