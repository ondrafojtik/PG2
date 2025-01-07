#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Subject.h"

struct cameraBounds
{
	float left, right, bottom, top;
	float getWidth()  { return right - left; }
	float getHeight() { return top - bottom; }
};

struct AspectRatio
{
	int x, y;
    
	AspectRatio(int x, int y)
	{
		this->x = x;
		this->y = y;
	}
};

struct Camera : public Subject
{

    void MoveUp()       { m_Position += (speed * up);					RecalcView(); }
	void MoveDown()     { m_Position -= (speed * up);					RecalcView(); }
	void MoveLeft()     { m_Position -= (speed * rotationProduct);		RecalcView(); }
	void MoveRight()    { m_Position += (speed * rotationProduct);		RecalcView(); }
	void MoveForward()  { m_Position += (speed * forward);				RecalcView(); }
	void MoveBackward() { m_Position -= (speed * forward);				RecalcView(); }

	void mouseUpdate(const glm::dvec2& newMousePosition);

	Camera(const glm::vec3& position, float fov, AspectRatio ar, float zNear, float zFar);

	glm::mat4 GetProjection() { return perspective; }

	glm::mat4 GetView() { return view; }

	void RecalcView() { view = glm::lookAt(m_Position, m_Position + forward, up); NotifyObservers(); }

	glm::vec3 GetPosition() { return m_Position; }

	glm::vec3 GetForward() { return forward; }

	float GetNear() { return m_near; }
	float GetFar() { return m_far; }

	// observer
	virtual void NotifyObservers()
	{
		for (Observer* obs : Subject::observers)
			obs->Update(perspective, view, m_Position);
	}

private:
	glm::vec3 m_Position;
	glm::mat4 perspective;
	glm::mat4 view;

	glm::vec3 forward;
	glm::vec3 up;
	
	glm::vec3 rotationProduct;

	glm::dvec2 oldMousePos;
	float speed = 0.2f;

	float m_near;
	float m_far;

};
