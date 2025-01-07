#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "Observer.h"

// this is bad! its here because i thought Id be making "minecraft clone", so rendering cubes was the only thing that mattered.. 
// it really need rebuilding..

struct RenderData : Observer
{
	const int vertexCount = 24;
	const int vertexInfo = 3 + 2 + 3;	//not "vertexCount, but how much info does 1 vertex carry (3 + 2) - you should make this more "normal"

	glm::vec3 positions[24];
	glm::vec2 texCoords[24];
	glm::vec3 normal[24];

	unsigned int vao;
	VertexBuffer* vb = nullptr;
	VertexBufferLayout layout;
	VertexArray va;
	IndexBuffer* ib = nullptr;
	//make this into some sorta std::unordered_map<std::string, Shader> or smth (and keep it here)
	//Shader* shader = nullptr;
	//Shader* shader_lightning = nullptr;
	std::unordered_map<std::string, Shader*> shaders;

	VertexBuffer* unchanged_vb = nullptr;
	VertexArray unchanged_va;

	void Init();

	void RefreshData()
	{
		float pos[(3 + 2 + 3) * 24];
		for (int i = 0; i < vertexCount; i++)
		{
			pos[i * vertexInfo + 0] = positions[i].x;
			pos[i * vertexInfo + 1] = positions[i].y;
			pos[i * vertexInfo + 2] = positions[i].z;
			pos[i * vertexInfo + 3] = texCoords[i].x;
			pos[i * vertexInfo + 4] = texCoords[i].y;
			pos[i * vertexInfo + 5] = normal[i].x;
			pos[i * vertexInfo + 6] = normal[i].y;
			pos[i * vertexInfo + 7] = normal[i].z;
		}

		vb->RefreshBuffer(pos);
		va.AddBuffer(*vb, layout);
	}

	virtual void Update(glm::mat4 projection, glm::mat4 view, glm::vec3 position)
	{
		for (std::pair<std::string, Shader*> s : shaders)
		{
			//s.second->Bind();
			//s.second->SetUniformMat4f("u_Projection", projection);
			//s.second->SetUniformMat4f("u_View", view);
			//s.second->SetUniform3f("u_CameraPos", position.x, position.y, position.z);
		}
	}


};
