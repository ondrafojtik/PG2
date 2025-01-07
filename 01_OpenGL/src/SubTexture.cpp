#include "SubTexture.h"

SubTexture::SubTexture(Texture& texture, glm::vec2 spriteSize, float x, float y)
{
	m_texture = &texture;
	
	texCoords[0] = { (x * spriteSize.x) / texture.GetWidth(), (y * spriteSize.y) / texture.GetHeight() };
	texCoords[1] = { ((x + 1) * spriteSize.x) / texture.GetWidth(), (y * spriteSize.y) / texture.GetHeight() };
	texCoords[2] = { ((x + 1) * spriteSize.x) / texture.GetWidth(), ((y + 1) * spriteSize.y) / texture.GetHeight() };
	texCoords[3] = { (x * spriteSize.x) / texture.GetWidth(), ((y + 1) * spriteSize.y) / texture.GetHeight() };
	
}
