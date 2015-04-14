#include "texture.h"
#include <cstring> //memcpy

size_t gl_pixel_size(GLint source_format, GLint component_type)
{
	unsigned component_count = 0;
	unsigned component_size = 0;
	
	switch(source_format) {
		case GL_RED: 
		case GL_RED_INTEGER: 
		case GL_STENCIL_INDEX: 
		case GL_DEPTH_COMPONENT: 
			component_count = 1;
		break;
		
		case GL_RG:
		case GL_RG_INTEGER: 
		case GL_DEPTH_STENCIL: 
			component_count = 2;
		break;
		
		case GL_RGB: 
		case GL_BGR: 
		case GL_RGB_INTEGER: 
		case GL_BGR_INTEGER: 
			component_count = 3;
		break;
		
		case GL_RGBA: 
		case GL_BGRA: 
		case GL_RGBA_INTEGER:
		case GL_BGRA_INTEGER: 
			component_count = 4;
		break;
	}
	
	switch(component_type) {
		case GL_UNSIGNED_BYTE: 
		case GL_BYTE: 
		case GL_UNSIGNED_BYTE_3_3_2: 
		//case GL_UNSIGNED_BYTE_3_3_2_REV: 
			component_size = 1;
		break;
		
		case GL_UNSIGNED_SHORT: 
		case GL_SHORT: 
		case GL_UNSIGNED_SHORT_5_6_5: 
		case GL_UNSIGNED_SHORT_5_6_5_REV: 
		case GL_UNSIGNED_SHORT_4_4_4_4:
		case GL_UNSIGNED_SHORT_4_4_4_4_REV:
		case GL_UNSIGNED_SHORT_5_5_5_1:
		//case GL_UNSIGNED_SHORT_5_5_5_1_REV:
			component_size = 2;
		break;
		
		case GL_UNSIGNED_INT: 
		case GL_INT: 
		case GL_FLOAT: 
		case GL_UNSIGNED_INT_8_8_8_8:
		case GL_UNSIGNED_INT_8_8_8_8_REV: 
		case GL_UNSIGNED_INT_10_10_10_2: 
		//case GL_UNSIGNED_INT_10_10_10_2_REV:
			component_size = 4;
		break;
	}
	
	return component_count * component_size;
};

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
	m_Image = image(data, w*h*gl_pixel_size(source_format, component_type), w, h, source_format, component_type);
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

void texture_cube::upload_internal(GLint gl_face, unsigned int_face)
{
	if(!this->is_valid())
		this->create_texture();
	
	this->use();
	glTexImage2D(gl_face, 0, m_InternalFormat, m_Images[int_face].width, m_Images[int_face].height,0, m_Images[int_face].data_format, m_Images[int_face].data_type, m_Images[int_face].data);
}

void texture_cube::upload(GLenum face, const image& img, GLint internal_format)
{
	unsigned face_ind = 8;
	
	switch(face){
		case GL_TEXTURE_CUBE_MAP_POSITIVE_X: 
			face_ind = 0;
		break;
			
		case GL_TEXTURE_CUBE_MAP_NEGATIVE_X: 
			face_ind = 1;
		break;
			
		case GL_TEXTURE_CUBE_MAP_POSITIVE_Y: 
			face_ind = 2;
		break;
		
		case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y: 
			face_ind = 3;
		break;
		
		case GL_TEXTURE_CUBE_MAP_POSITIVE_Z: 
			face_ind = 4;
		break;
		
		case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z: 
			face_ind = 5;
		break;
		
		default: 
			return;
	}
	
	m_Images[face_ind] = img;
	m_InternalFormat = internal_format;
	this->upload_internal(face, face_ind);
}

void texture_cube::upload(GLenum face, void* data, unsigned w, unsigned h, GLint internal_format, GLint source_format, GLint component_type)
{
	if(!this->is_valid())
		create_texture();
	
	this->use();
	glTexImage2D(face, 0, internal_format, w,h,0, source_format, component_type, data);
}

void texture_cube::parameter(GLenum param, int value) { glTexParameteri(GL_TEXTURE_CUBE_MAP, param, value); }
void texture_cube::parameter(GLenum param, float value) { glTexParameterf(GL_TEXTURE_CUBE_MAP, param, value); }
void texture_cube::generate_mipmaps() { this->use(); glGenerateMipmap(GL_TEXTURE_CUBE_MAP); }
void texture_cube::use() const { glBindTexture(GL_TEXTURE_CUBE_MAP, m_Texture); }

void texture_cube::free() 
{
	glDeleteTextures(1, &m_Texture);
	m_Texture = 0;
}