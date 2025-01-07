#pragma once

class Observer
{
public:
	virtual void Update(glm::mat4 projection, glm::mat4 view, glm::vec3 position) = 0;
};