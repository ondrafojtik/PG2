#pragma once
#include <vector>
//#include "Particle.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>

#include "glm/vec2.hpp"
#include "glm/vec4.hpp"
#include "Random.h"

struct Particle
{
	float x, y;
	float life;
	float currLife;
	glm::vec4 startingColor;
	glm::vec4 dyingColor;
	glm::vec2 size;

	glm::vec4 color;
	glm::vec2 velocity;

	Particle(float xPos, float yPos, float lifePoints, glm::vec4 particleStartingColor, glm::vec4 particleDyingColor, glm::vec2 particleSize)
		: x(xPos), y(yPos), life(lifePoints), startingColor(particleStartingColor), dyingColor(particleDyingColor), size(particleSize), currLife(lifePoints)
	{
		velocity = glm::vec2(Random::Float() * 100 - 50, Random::Float() * 100 - 50);
	}

	bool Alive() { return currLife > 0; }

	void Update()
	{
		currLife -= 1;
		float _life = currLife / life;
		color = glm::lerp(dyingColor, startingColor, _life);
		color.a = color.a * _life;
		
		//fun bug (with no buffer swap and on release - basically need like 1k fps)
		//size.x -= 0.1f;
		//size.y -= 0.1f;


		size.x = glm::lerp(0.0f, size.x, _life);
		size.y = glm::lerp(0.0f, size.y, _life);

		float speed = 1.0f;
		x += velocity.x * speed / 100;
		y += velocity.y * speed / 100;


	}

};

struct ParticleSystem
{
	std::vector<Particle> buffer;
	
	void Init()
	{
		buffer.reserve(1000);
	}
	void Add(float x, float y, float life, glm::vec4 startingColor, glm::vec4 dyingColor, glm::vec2 size) { buffer.emplace_back(x, y, life, startingColor, dyingColor, size); }
	
	void Update()
	{
		for (int i = 0; i < buffer.size(); i++)
		{
			buffer[i].Update();
			if (!buffer[i].Alive())
				buffer.erase(buffer.begin() + i);
		}
	}

};