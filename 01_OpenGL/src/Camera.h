#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Subject.h"
#include "glm/mat4x4.hpp"
#include "glm/gtc/type_ptr.hpp"

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

    //void MoveUp()       { m_Position += (speed * up);					RecalcView(); }
	//void MoveDown()     { m_Position -= (speed * up);					RecalcView(); }
	//void MoveLeft()     { m_Position -= (speed * rotationProduct);		RecalcView(); }
	//void MoveRight()    { m_Position += (speed * rotationProduct);		RecalcView(); }
	//void MoveForward()  { m_Position += (speed * forward);				RecalcView(); }
	//void MoveBackward() { m_Position -= (speed * forward);				RecalcView(); }

	void MoveUp() { m_Position += (speed * up);						UpdateView(); }
	void MoveDown() { m_Position -= (speed * up);					UpdateView(); }
	void MoveLeft() { m_Position += (speed * rotationProduct);		UpdateView(); }
	void MoveRight() { m_Position -= (speed * rotationProduct);		UpdateView(); }
	void MoveForward() { m_Position += (speed * forward);			UpdateView(); }
	void MoveBackward() { m_Position -= (speed * forward);			UpdateView(); }

	void mouseUpdate(const glm::dvec2& newMousePosition);

	Camera(const glm::vec3& position, float fov, AspectRatio ar, float zNear, float zFar);
	Camera(const int width, const int height, const float fov_y, const glm::vec3 view_from, const glm::vec3 view_at);

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
		//for (Observer* obs : Subject::observers)
		//	obs->Update(perspective, view, m_Position);
		for (Observer* obs : Subject::observers)
			obs->Update(ProjectionMatrix, ViewMatrix, m_Position);
	}

	glm::vec3 m_Position;
private:
	glm::mat4 perspective;
	glm::mat4 view;

	glm::vec3 forward;
	glm::vec3 up;
	
	glm::vec3 rotationProduct;

	glm::dvec2 oldMousePos;
	float speed = 0.2f;

	float m_near;
	float m_far;



	// PG2
public:
	glm::vec3 view_from() const;
	glm::mat3 M_c_w() const;
	float focal_length() const;

	void set_fov_y(const float fov_y);

	void set_view_at(glm::vec3 view_at)
	{
		view_at_ = view_at;
		m_Position = view_from_;
		Update();
		NotifyObservers();
	}

	void set_view_from(glm::vec3 view_from)
	{
		view_from_ = view_from;
		m_Position = view_from;
		Update();
		NotifyObservers();
	}

	void Update();
	void UpdateView();
	void UpdateProjection();


	glm::mat4 get_projection();
	glm::mat4 get_view();



	//void MoveForward(const float dt);

	int width_{ 640 }; // image width (px)
	int height_{ 480 };  // image height (px)
	float fov_y_{ 0.785f }; // vertical field of view (rad)

	glm::vec3 view_from_; // ray origin or eye or O
	glm::vec3 view_at_; // target T
	glm::vec3 up_{ glm::vec3(0.0f, 0.0f, 1.0f) }; // up vector

	float f_y_{ 1.0f }; // focal lenght (px)

	glm::mat3 M_c_w_; // transformation matrix from CS -> WS	

	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;

	float nearProjection = 1.0; // 0.1f
	float farProjection = 1000.0; // 100.0f

};
