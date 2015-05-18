#ifndef _H_FBODEV_H_
#define _H_FBODEV_H_

#include <GL/glew.h>

class fbo {
	private: 
		struct attachment_t {
			GLuint id;
			GLenum type;
		};

	private:
		GLuint m_Handle = 0;

		void create();

	public: 
		bool 	is_valid() const;
		GLenum 	check_status() const;

		void 	bind(GLenum type = GL_FRAMEBUFFER);

		void	attach_texture(GLenum attachment_point, GLenum texture_type, GLuint texture, GLint texture_level, bool own = 1);
		void 	attach_rbo(GLenum attachment_point, );

		bool	free();
};

#endif