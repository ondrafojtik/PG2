#pragma once
#include <iostream>
#include <vector>

#include "glm/vec2.hpp"
#include "SubTexture.h"

/*
this animation only works, if all the frames are in 1line. 

length = 4
| A | B | C | D |

*/

struct Animation
{
	SubTexture* currentAnimation = nullptr;
	//length meaninig how many "frames" its got
	Animation(Texture& texture, glm::vec2 spriteSize, int length, float animationSpeed)
		: m_animationSpeed(animationSpeed), m_length(length)
	{
		for(int i = 0; i < length; i++)
		{
			SubTexture* sub = new SubTexture(texture, spriteSize, i, 0);
			animation.push_back(sub);
		}
	}

	~Animation()
	{
		for (auto sub : animation)
			delete sub;
	}

	void OnUpdate()
	{
		currPos += m_animationSpeed;
		if (currPos > m_length)
			currPos = 0.0f;
		currentAnimation = animation[(int)currPos];
	}

private: 
	std::vector<SubTexture*> animation;
	float m_animationSpeed;
	int m_length = 0;

	float currPos = 0.0f;

};