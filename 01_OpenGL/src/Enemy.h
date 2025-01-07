#pragma once

#include "RenderData.h"
#include "Object.h"
#include "glm/gtc/matrix_transform.hpp"

class Enemy : public Object
{
public:
	Enemy(glm::mat4x3 bezierPoints);
	~Enemy();

	virtual glm::mat4 GetTransform() { return transform; }

	virtual Shader* GetShader() { return new Shader("src/res/shaders/plainColor.shader"); }
	virtual VertexBuffer* GetVB() { return vb; }
	virtual VertexArray* GetVA() { return va; }
	virtual IndexBuffer* GetIB() { return ib; }

	virtual void Translate(glm::vec3 newPosition) { translate = glm::translate(glm::mat4(1.0f), newPosition); Recalculate(); }
	virtual void Rotate(float angle, glm::vec3 axis) { rotate = glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis); Recalculate(); }
	virtual void Scale(glm::vec3 dimension) { scale = glm::scale(glm::mat4(1.0f), dimension); Recalculate(); }

	virtual glm::vec3 GetColor() { return glm::vec3(1.0f, 0.0f, 0.0f); }
	virtual glm::vec3 GetPosition() { return this->final; }
	
	virtual void SetID(int id) { this->id = id; }
	virtual int GetID() { return id; }

	virtual void Move(float speed);

private:
	const int vertexCount = 24;
	const int vertexInfo = 3 + 2 + 3;

	int id;

	float timeStep = 0.0f;
	int direction = 1;
	glm::vec3 final = glm::vec3(0.0f);
	glm::vec4 matOne = glm::vec4(0.0f);
	glm::mat4 matTwo = {
		-1,  3, -3, 1,
		 3, -6,  3, 0,
		-3,  3,  0, 0,
		 1,  0,  0, 0
	};
	glm::mat4x3 matThree = {
		0, 5, 0,
		0, 5, 10,
		10, 5, 10,
		10, 5, 0
	};

	unsigned int vao;
	VertexBuffer* vb = nullptr;
	VertexBufferLayout layout;
	VertexArray* va = nullptr;
	IndexBuffer* ib = nullptr;

	glm::mat4 translate = glm::mat4(1.0f);
	glm::mat4 rotate = glm::mat4(1.0f);
	glm::mat4 scale = glm::mat4(1.0f);

	glm::mat4 transform = glm::mat4(1.0f);

	void Recalculate()
	{
		transform = translate * rotate * scale;
	}

	void RecalcBezier();


};