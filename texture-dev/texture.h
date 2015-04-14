#ifndef _H_TEXTURE_H_
#define _H_TEXTURE_H_

#include <GL/glew.h>

struct image 
{
	char* data = NULL;
	size_t size = 0;
	
	unsigned width;
	unsigned height;
	
	GLenum data_format; //As in RGB, BGRA, etc.
	GLenum data_type;   //As in GL_FLOAT, GL_UNSIGNED_SHORT, etc.
	
	//
	
	image() = default;
	image(void* data, size_t size, unsigned w, unsigned h, GLint format, GLint type);
	image(const image&);
	image(image&&);
	~image();
	
	image& operator=(const image&);
};

class texture2d
{
	private: 
		GLuint	m_Texture = 0;
		image	m_Image;
		GLint 	m_InternalFormat;
		
		void create_texture();
		void upload_internal();
		
	public: 
		bool is_valid() const;
		GLuint handle() const;
		GLuint operator()() const;
		
		void upload(void* data, unsigned w, unsigned h, GLint internal_format, GLint source_format, GLint component_type);
		void upload(const image& img, GLint internal_format);
		void parameter(GLenum param, int value);
		void parameter(GLenum param, float value);
		void generate_mipmaps();
		void use() const;
		
		void free();
};

class texture_cube
{
	private: 
		GLuint	m_Texture = 0;
		image	m_Images[6];
		GLint 	m_InternalFormat;
		
		void create_texture();
		void upload_internal(GLint gl_face, unsigned int_face);
		
	public: 
		bool is_valid() const;
		GLuint handle() const;
		GLuint operator()() const;
		
		void upload(GLenum face, void* data, unsigned w, unsigned h, GLint internal_format, GLint source_format, GLint component_type);
		void upload(GLenum face, const image& img, GLint internal_format);
		void parameter(GLenum param, int value);
		void parameter(GLenum param, float value);
		void generate_mipmaps();
		void use() const;
		
		void free();
};

#endif