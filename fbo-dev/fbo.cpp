#include "fbo.h"

void fbo::create() {
	glGenFramebuffers(1, &m_Handle);
}

bool fbo::is_valid() const {
	return m_Handle != 0;
}

//!!!CHANGES GL STATE!!!
GLenum fbo::check_status() const {
	glBindFramebuffer(GL_FRAMEBUFFER);
	return glCheckFramebufferStatus(GL_FRAMEBUFFER);
}

void fbo::bind(GLenum type) {
	if(!is_valid())
		create();

	m_BindType = type;
	glBindFramebuffer(type, m_Handle);
}

//TODO: extend 3D and cube map support
void fbo::attach_texture(GLenum attachment_point, GLenum texture_type, GLuint texture, GLint texture_level, bool own) {
	bind(m_BindType);
	glBindTexture(texture_type, texture);

	bool attach_success = 1;
	switch(texture_type) {
		case GL_TEXTURE_1D: glFramebufferTexture1D(m_BindType, attachment_point, texture_type, texture, 0); break;
		case GL_TEXTURE_2D: glFramebufferTexture2D(m_BindType, attachment_point, texture_type, texture, 0); break;
		case GL_TEXTURE_3D: glFramebufferTexture1D(m_BindType, attachment_point, texture_type, texture, , 0); break;
		default: attach_success = 0;
	}

	if(attach_succes && own) 
		m_Attachments.insert({texture_type, texture});
}

void fbo::attach_rbo(GLenum attachment_point, GLuint rbo, bool own) {
	bind(m_BindType);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glFramebufferRenderbuffer(m_BindType, attachment_point, GL_RENDERBUFFER, rbo);

	if(own)
		m_Attachments.insert({GL_RENDERBUFFER, rbo});
}

bool fbo::free() {
	if(!is_valid())
		return false;

	for(const attachment_t& a : m_Attachments)
	{
		switch(a.first) {
			case GL_TEXTURE_1D:
			case GL_TEXTURE_2D:
			case GL_TEXTURE_3D:
				glDeleteTextures(1, &a.second);
			break;

			case GL_RENDERBUFFER:
				glDeleteRenderbuffers(1, &a.second);
			break;
		}
	}

	return true;
}