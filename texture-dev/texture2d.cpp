#include "texture2d.h"

void texture2d::create_texture() { glGenTextures(1, &m_Texture); } 

bool texture2d::is_valid() const { return m_Texture != 0;}
GLuint texture2d::handle() const { return m_Texture; } 
GLuint texture2d::operator()() const { return m_Texture; }

void texture2d::upload(void* data, unsigned w, unsigned h, GLint internal_format, GLint source_format, GLint component_type)
{
	if(!this->is_valid())
		create_texture();
	
	this->use();
	glTexImage2D(GL_TEXTURE_2D, 0, internal_format, w,h,0, source_format, component_type, data);
}

void texture2d::parameter(GLenum param, int value) { glTexParameteri(GL_TEXTURE_2D, param, value); }
void texture2d::parameter(GLenum param, float value) { glTexParameterf(GL_TEXTURE_2D, param, value); }
void texture2d::use() const { glBindTexture(GL_TEXTURE_2D, m_Texture); }

void texture2d::free() 
{
	glDeleteTextures(1, &m_Texture);
	m_Texture = 0;
}