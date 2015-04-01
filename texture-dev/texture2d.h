#ifndef _H_TEXTURE2D_H_
#define _H_TEXTURE2D_H_

#include <GL/glew.h>

class texture2d
{
	private: 
		GLuint m_Texture = 0;
		unsigned m_Width;
		unsigned m_Height;
		unsigned m_Components;
		
		GLenum m_DataFormat;
		GLenum m_DataLayout;
		
		GLenum m_InternalFormat;
		GLenum m_InternalLayout;
		
		void create_texture();
		
	public: 
		bool is_valid() const;
		GLuint handle() const;
		GLuint operator()() const;
		
		void upload(void* data, unsigned w, unsigned h, GLint internal_format, GLint source_format, GLint component_type);
		void parameter(GLenum param, int value);
		void parameter(GLenum param, float value);
		void use() const;
};

#endif