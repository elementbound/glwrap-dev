#ifndef _H_TEXTURE_H_
#define _H_TEXTURE_H_

class texture
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
		
	public: 
		bool is_valid();
		GLuint handle() const;
		GLuitn operator()() const;
		
		void upload(void* data, unsigned w, unsigned h, unsigned components, unsigned layout, unsigned type);
		void use();
};

#endif