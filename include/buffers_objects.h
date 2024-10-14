#pragma once

#include <vector>
#include "renderer.h"

class VBO {
	GLuint m_ID;
public:
	VBO(const void* data, GLuint size);
	~VBO();

	void bind() const;
	void unbind() const;
};

class IBO {
	GLuint m_ID;
	GLuint m_Count;
public:
	IBO(const GLuint* data, GLuint count);
	~IBO();

	void bind() const;
	void unbind() const;

	inline GLuint getCount() const { return m_Count; }
};
