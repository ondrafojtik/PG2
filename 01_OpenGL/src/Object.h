#pragma once

#include "Shader.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"

class Object
{
public:
	
	virtual glm::mat4 GetTransform() = 0;						
	
	virtual Shader* GetShader() = 0;							
	virtual VertexBuffer* GetVB() = 0;							
	virtual VertexArray* GetVA() = 0;							
	virtual IndexBuffer* GetIB() = 0;							

	virtual void Translate(glm::vec3 newPosition) = 0;			
	virtual void Rotate(float angle, glm::vec3 axis) = 0;	    
	virtual void Scale(glm::vec3 dimension) = 0;			    
	
	// type of object
	virtual glm::vec3 GetColor() = 0;
	virtual glm::vec3 GetPosition() = 0;

	// for item detection
	virtual void SetID(int id) = 0;
	virtual int GetID() = 0;

	// for movement
	virtual void Move(float speed) = 0;
};