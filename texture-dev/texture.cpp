#include "texture.h"
#include <cstring> //memcpy

//=============================================================================================
//image

image::image(void* data, size_t size, unsigned w, unsigned h, GLint format, GLint type) : 
	data((char*)data), size(size), 
	width(w), height(h),
	data_format(format), data_type(type)
	{}
	
//TODO: Possibly implement copy-on-write
image::image(const image& rhs)
{
	width = rhs.width;
	height = rhs.height;
	size = rhs.size;
	
	data_format = rhs.data_format;
	data_type = rhs.data_type;
	
	data = new char[size];
	memcpy(data, rhs.data, size);
}

image::image(image&& rhs)
{
	data = rhs.data;
	
	width = rhs.width;
	height = rhs.height;
	size = rhs.size;
	
	data_format = rhs.data_format;
	data_type = rhs.data_type;
	
	//
	
	rhs.data = NULL;
	rhs.size = 0;
	
	rhs.width = 0;
	rhs.height = 0;
}

image::~image()
{
	if(data != NULL)
		delete [] data;
}

image& image::operator=(const image& rhs)
{
	if(data != NULL)
		delete [] data;
	
	width = rhs.width;
	height = rhs.height;
	size = rhs.size;
	
	data_format = rhs.data_format;
	data_type = rhs.data_type;
	
	data = new char[size];
	memcpy(data, rhs.data, size);
}

//=============================================================================================
//texture2d

void texture2d::create_texture() { glGenTextures(1, &m_Texture); } 

bool texture2d::is_valid() const { return m_Texture != 0;}
GLuint texture2d::handle() const { return m_Texture; } 
GLuint texture2d::operator()() const { return m_Texture; }

void texture2d::upload_internal()
{
	if(!this->is_valid())
		create_texture();
	
	this->use();
	glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Image.width, m_Image.height,0, m_Image.data_format, m_Image.data_type, m_Image.data);
}

void texture2d::upload(const image& img, GLint internal_format) 
{
	m_Image = img;
	m_InternalFormat = internal_format;
	this->upload_internal();
}

void texture2d::upload(void* data, unsigned w, unsigned h, GLint internal_format, GLint source_format, GLint component_type)
{
	//TODO: figure out image size based on source_format and component_type
	#warning This function is WIP, produces memory leak atm
	m_Image = image(data, 0, w, h, source_format, component_type);
	m_InternalFormat = internal_format;
	this->upload_internal();
}

void texture2d::parameter(GLenum param, int value) { glTexParameteri(GL_TEXTURE_2D, param, value); }
void texture2d::parameter(GLenum param, float value) { glTexParameterf(GL_TEXTURE_2D, param, value); }
void texture2d::generate_mipmaps() { this->use(); glGenerateMipmap(GL_TEXTURE_2D); }
void texture2d::use() const { glBindTexture(GL_TEXTURE_2D, m_Texture); }

void texture2d::free() 
{
	glDeleteTextures(1, &m_Texture);
	m_Texture = 0;
}

//=============================================================================================
//texture_cube

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

void texture_cube::parameter(GLenum param, int value) { glTexParameteri(GL_TEXTURE_CUBE_MAP, param, value); }
void texture_cube::parameter(GLenum param, float value) { glTexParameterf(GL_TEXTURE_CUBE_MAP, param, value); }
void texture_cube::use() const { glBindTexture(GL_TEXTURE_CUBE_MAP, m_Texture); }

void texture_cube::free() 
{
	glDeleteTextures(1, &m_Texture);
	m_Texture = 0;
}