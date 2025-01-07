#pragma once

#include <string>
#include <vector>
#include <iostream>

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

#include "assimp\Importer.hpp"
#include "assimp\scene.h"
#include "assimp\postprocess.h"
#include "assimp/material.h"

#include "Texture.h"


namespace PG2
{
	struct Material
	{
		float Ns;
		float Ni;
		glm::vec3 Ka;
		glm::vec3 Kd;
		glm::vec3 Ks;
		glm::vec3 Ke;

		std::string name;
	};



	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 color;
		glm::vec2 texCoords = glm::vec2(5.0f, 5.0f);
		glm::vec3 tangent;
		glm::vec3 bitangent;
		glm::vec3 material_index;
	};


	struct TextureInfo
	{
		std::string name = "";
		std::string type = "";
	};

	struct Mesh
	{
		std::vector<PG2::Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<PG2::TextureInfo> textures;

		Mesh(std::vector<PG2::Vertex> vertices, std::vector<unsigned int> indices, std::vector<PG2::TextureInfo> textures)
		{
			this->vertices = vertices;
			this->indices = indices;
			this->textures = textures;

			setupMesh();
		}

		unsigned int vao;
		VertexBuffer* vb = nullptr;
		VertexBufferLayout layout;
		VertexArray* va = nullptr;
		IndexBuffer* ib = nullptr;

		void setupMesh()
		{
			GLCall(glGenVertexArrays(1, &vao));
			GLCall(glBindVertexArray(vao));

			vb = new VertexBuffer(&vertices[0], vertices.size() * sizeof(PG2::Vertex));
			layout.Push<float>(3);
			layout.Push<float>(3);
			layout.Push<float>(3);
			layout.Push<float>(2);
			layout.Push<float>(3);
			layout.Push<float>(3);
			layout.Push<float>(3);

			va = new VertexArray();
			va->AddBuffer(*vb, layout);
			ib = new IndexBuffer(&indices[0], indices.size());
		}

		void Draw()
		{
			/*
			for auto mesh in meshes
				draw mesh

			*/
		}

	};

	struct Model
	{

		Model(std::string path)
		{
			loadModel(path);
		}

		void Draw()
		{

		}

		std::vector<PG2::Mesh> meshes;
		std::string directory;
		std::vector<PG2::Material> materials;
		std::unordered_map<std::string, Texture*> textures;

		void loadModel(std::string path) 
		{
			Assimp::Importer import;
			const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate); //  | aiProcess_CalcTangentSpace

			if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
			{
				std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
				return;
			}
			directory = path.substr(0, path.find_last_of('/'));

			processNode(scene->mRootNode, scene);

			for (unsigned int i = 0; i < scene->mNumMaterials; i++)
			{
				aiMaterial* material = scene->mMaterials[i];

				aiColor4D diffuse;							// Kd
				aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuse);

				aiColor4D ambient;							// Ka
				aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &ambient);

				aiColor4D specular;							// Ks
				aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &specular);

				aiColor4D emission;							// Ke
				aiGetMaterialColor(material, AI_MATKEY_COLOR_EMISSIVE, &emission);

				aiColor4D spec_coef; // spec_coef.r			// Ns
				aiGetMaterialColor(material, AI_MATKEY_SHININESS, &spec_coef);

				aiColor4D refraction; // refraction.r		// Ni
				aiGetMaterialColor(material, AI_MATKEY_REFRACTI, &refraction);

				aiString name;
				material->Get(AI_MATKEY_NAME, name);

				PG2::Material _material;
				_material.Kd.x = diffuse.r;
				_material.Kd.y = diffuse.g;
				_material.Kd.z = diffuse.b;

				_material.Ka.x = ambient.r;
				_material.Ka.y = ambient.g;
				_material.Ka.z = ambient.b;

				_material.Ks.x = specular.r;
				_material.Ks.y = specular.g;
				_material.Ks.z = specular.b;

				_material.Ke.x = emission.r;
				_material.Ke.y = emission.g;
				_material.Ke.z = emission.b;

				_material.Ns = spec_coef.r;

				_material.Ni = refraction.r;

				_material.name = name.C_Str();

				materials.push_back(_material);
			}


			textures["blank"] = new Texture("src/res/textures/Blank.png");
			//textures["blank"] = new Texture("src/res/models/piece_02/plastic_02_rma.png");

		}

		void processNode(aiNode* node, const aiScene* scene)
		{
			for (unsigned int i = 0; i < node->mNumMeshes; i++)
			{
				aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
				meshes.push_back(processMesh(mesh, scene));
			}

			for (unsigned int i = 0; i < node->mNumChildren; i++)
			{
				processNode(node->mChildren[i], scene);
			}

			
		}

		PG2::Mesh processMesh(aiMesh* mesh, const aiScene* scene)
		{
			std::vector<PG2::Vertex> vertices;
			std::vector<unsigned int> indices;
			std::vector<PG2::TextureInfo> textures;

			// for vertices

			if (mesh->mNumVertices > 0)
			{
				PG2::Vertex vertex;
				aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];


				for (int i = 0; i < mesh->mNumVertices; i++)
				{
					vertex.position.x = mesh->mVertices[i].x;
					vertex.position.y = mesh->mVertices[i].y;
					vertex.position.z = mesh->mVertices[i].z;

					if (mesh->HasNormals())
					{
						vertex.normal.x = mesh->mNormals[i].x;
						vertex.normal.y = mesh->mNormals[i].y;
						vertex.normal.z = mesh->mNormals[i].z;
					}

					if (mesh->mTextureCoords[0])
					{
						vertex.texCoords.x = mesh->mTextureCoords[0][i].x;
						vertex.texCoords.y = mesh->mTextureCoords[0][i].y;
					}

					if (mesh->HasTangentsAndBitangents())
					{
						//vertex.tangent.x = mesh->mTangents[i].x;
						//vertex.tangent.y = mesh->mTangents[i].y;
						//vertex.tangent.z = mesh->mTangents[i].z;
						//
						//vertex.bitangent.x = mesh->mBitangents[i].x;
						//vertex.bitangent.y = mesh->mBitangents[i].y;
						//vertex.bitangent.z = mesh->mBitangents[i].z;
					}

					vertex.material_index = { (float)mesh->mMaterialIndex, 0, 0 };

					aiColor4D diffuse;							// Kd
					aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuse);
					vertex.color.x = diffuse.r;
					vertex.color.y = diffuse.g;
					vertex.color.z = diffuse.b;
 
					aiColor4D ambient;							// Ka
					aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &ambient);
					
					aiColor4D specular;							// Ks
					aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &specular);

					aiColor4D emission;							// Ke
					aiGetMaterialColor(material, AI_MATKEY_COLOR_EMISSIVE, &emission);

					aiColor4D spec_coef; // spec_coef.r			// Ns
					aiGetMaterialColor(material, AI_MATKEY_SHININESS, &spec_coef);

					aiColor4D refraction; // refraction.r		// Ni
					aiGetMaterialColor(material, AI_MATKEY_REFRACTI, &refraction);

					vertices.push_back(vertex);
				}

				for (int i = 0; i < mesh->mNumFaces; i++)
				{
					aiFace face = mesh->mFaces[i];
					
					std::vector<unsigned int> _triangle;
					for (unsigned int j = 0; j < face.mNumIndices; j++)
					{
						indices.push_back(face.mIndices[j]);
						_triangle.push_back(face.mIndices[j]);
					}

					// compute T & B
					Vertex v0 = vertices[_triangle[0]];
					Vertex v1 = vertices[_triangle[1]];
					Vertex v2 = vertices[_triangle[2]];

					glm::vec3 e1 = v1.position - v0.position;
					glm::vec3 e2 = v2.position - v0.position;

					glm::vec2 delta_uv1 = v1.texCoords - v0.texCoords;
					glm::vec2 delta_uv2 = v2.texCoords - v0.texCoords;

					float div_ = 1 / ((delta_uv1.x * delta_uv2.y) - (delta_uv2.x * delta_uv1.y));
					
					float tx = div_ * (e1.x * delta_uv2.y - e2.x * delta_uv1.y);

					glm::vec3 t = div_ * (e1 * delta_uv2.y - e2 * delta_uv1.y);
					glm::vec3 b = div_ * (e2 * delta_uv1.x - e1 * delta_uv2.x);
					//glm::vec3 n = glm::normalize(v0.normal);
					//
					//glm::vec3 t_ = glm::normalize(t - (n * glm::dot(t, n)));
					//glm::vec3 b_ = glm::normalize(b - (n * glm::dot(b, n)) - (t_ * glm::dot(b, t_)));

					for (unsigned int j = 0; j < 3; j++)
					{
						glm::vec3 n = glm::normalize(vertices[_triangle[j]].normal);
						glm::vec3 t_ = glm::normalize(t - (n * glm::dot(t, n)));
						glm::vec3 b_ = glm::normalize(b - (n * glm::dot(b, n)) - (t_ * glm::dot(b, t_)));

						vertices[_triangle[j]].tangent = t_;
						vertices[_triangle[j]].bitangent = b_;
						//vertices[_triangle[j]].normal = glm::normalize(vertices[_triangle[j]].normal);
					}

				}

				
				std::vector<PG2::TextureInfo> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
				textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
				std::vector<PG2::TextureInfo> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
				textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
				std::vector<PG2::TextureInfo> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
				textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
				std::vector<PG2::TextureInfo> rmaMaps = loadMaterialTextures(material, aiTextureType_METALNESS, "texture_rma"); // map_Pm
				textures.insert(textures.end(), rmaMaps.begin(), rmaMaps.end());
				
				
					
			}


			return PG2::Mesh(vertices, indices, textures);

		}

		std::vector<PG2::TextureInfo> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
		{
			std::vector<PG2::TextureInfo> textures_;
			for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
			{
				aiString str;
				mat->GetTexture(type, i, &str);
				std::string texture_name = str.C_Str();

				std::string complete_path = directory + "/" + texture_name;
				
				if (textures.find(texture_name) == textures.end())
					textures[texture_name] = new Texture(complete_path, 1);
				PG2::TextureInfo texture;
				texture.name = texture_name;
				texture.type = typeName;
				textures_.push_back(texture);
			}

			return textures_;
		}


	};




}