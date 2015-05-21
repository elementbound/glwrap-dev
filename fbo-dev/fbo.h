#ifndef _H_FBO_H_
#define _H_FBO_H_

#include <GL/glew.h>
#include <set>

class fbo {
	private: 
		typedef std::pair<GLenum, GLuint> attachment_t;

	private:
		GLuint m_Handle = 0;
		GLenum m_BindType = GL_FRAMEBUFFER;
		std::set<attachment_t> m_Attachments;

		void create();

	public: 
		bool 	is_valid() const;
		GLenum 	check_status() const;

		void 	bind(GLenum type = GL_FRAMEBUFFER);
		void	unbind();

		void	attach_texture(GLenum attachment_point, GLenum texture_type, GLuint texture, GLint texture_level, bool own = 1);
		void 	attach_rbo(GLenum attachment_point, GLuint rbo, bool own = 1);

		bool	free();
};

#endif