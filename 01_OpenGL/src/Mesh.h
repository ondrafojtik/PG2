#pragma once
#include <vector>

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Texture.h"

#include "glm/vec3.hpp"
#include "glm/vec2.hpp"

//TODO: dont need the Texture stuff in the constructor I guess (since I do it manually.. )
//mb figure out how to not do it "manually" 

struct Vertex
{
	glm::vec3 position;
	glm::vec2 texCoord;
	glm::vec3 normal;
	glm::vec3 tangent;
	glm::vec3 bitangent;
};

struct Mesh
{
    //vertexInfo == sizeof(Vertex)/sizeof(float) - correct only if all the members of Vertex r float 
	const int vertexInfo = 3 + 2 + 3 + 3 + 3;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	unsigned int vao;
	VertexBuffer* vb = nullptr;
	VertexBufferLayout layout;
	VertexArray* va = nullptr;
	IndexBuffer* ib = nullptr;

	Mesh(const std::vector<Vertex>& _vertices,
         const std::vector<unsigned int>& _indices)//,
        //const std::vector<Texture>& _textures)
		: vertices(_vertices), indices(_indices)//, textures(_textures)
	{
		setupMesh();
	}

	void setupMesh()
	{
		GLCall(glGenVertexArrays(1, &vao));
		GLCall(glBindVertexArray(vao));

		vb = new VertexBuffer(&vertices[0], vertices.size() * sizeof(Vertex));
		layout.Push<float>(3);
		layout.Push<float>(2);
		layout.Push<float>(3);
		layout.Push<float>(3);
		layout.Push<float>(3);


		va = new VertexArray();
		va->AddBuffer(*vb, layout);
        ib = new IndexBuffer(&indices[0], indices.size());
    }
	
};
