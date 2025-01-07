#pragma once
#include "glm/vec2.hpp"
#include "Texture.h"



struct SubTexture
{
	Texture* m_texture = nullptr;
	glm::vec2 texCoords[4];
	
	SubTexture(Texture& texture, glm::vec2 spriteSize, float x, float y);
	~SubTexture() { ; }
	
};
