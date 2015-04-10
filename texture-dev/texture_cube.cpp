#include "texture_cube.h"

void texture_cube::create_texture() { glGenTextures(1, &m_Texture); } 

bool texture_cube::is_valid() const { return m_Texture != 0;}
GLuint texture_cube::handle() const { return m_Texture; } 
GLuint texture_cube::operator()() const { return m_Texture; }

void texture_cube::upload(GLenum face, void* data, unsigned w, unsigned h, GLint internal_format, GLint source_format, GLint component_type)
{
	if(!this->is_valid())
		create_texture();
	
	this->use();
	glTexImage2D(face, 0, internal_format, w,h,0, source_format, component_type, data);
}

void texture_cube::parameter(GLenum param, int value) { glTexParameteri(GL_TEXTURE_CUBE_MAP​, param, value); }
void texture_cube::parameter(GLenum param, float value) { glTexParameterf(GL_TEXTURE_CUBE_MAP​, param, value); }
void texture_cube::use() const { glBindTexture(GL_TEXTURE_CUBE_MAP​, m_Texture); }

void texture_cube::free() 
{
	glDeleteTextures(1, &m_Texture);
	m_Texture = 0;
}