#pragma once

#include "stb-master\stb_image.h"

#include <iostream>
#include "GLcall.h"
#include <sstream>

struct OriginalTextureSize
{
	int width, height;
};

class Texture
{
private:
	
	int m_Width, m_Height, m_BPP;
	OriginalTextureSize m_OrignalSize;
	
public:

	unsigned char* m_LocalBuffer;
	Texture(const std::string& path);
	Texture(const std::string& path, bool flip_image);
	
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;
	
	unsigned int GetTexID() { return m_RendererID; }
	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
	OriginalTextureSize GetOriginalSize() const { return m_OrignalSize; }

	// pg2
	unsigned int m_RendererID;
	GLuint fbo_shadow_map_;
	Texture(const int shadow_width);
};
