#include "Model.h"
#include <fstream>

void Model::LoadModel(std::string path, bool flip_uv)
{
    // TODO: flip UVs..
    Assimp::Importer import;
    if (!flip_uv)
    {
        const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
			return;
		}
		directory = path.substr(0, path.find_last_of('/'));

		ProcessNode(scene->mRootNode, scene);

    }
    else
    {
		const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
			return;
		}
		directory = path.substr(0, path.find_last_of('/'));

		ProcessNode(scene->mRootNode, scene);

    }
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
    for (int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(ProcessMesh(mesh, scene));
    }

    for (int i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene);
    }

}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    //std::vector<Texture> textures;
    
    //vertex info set
    for (int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        //position and texCoords
        vertex.position =
            { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
        if (mesh->mTextureCoords[0]) //apparently "if has texCoords"
            vertex.texCoord =
                { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
        else
            vertex.texCoord = { 0.0f, 0.0f };
        //TBN
        if(mesh->HasNormals())
            vertex.normal =
                           { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };
        if (mesh->HasTangentsAndBitangents())
        {
            vertex.tangent =   { mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z };
            vertex.bitangent = { mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z  };
        }
        vertices.push_back(vertex);
    }

    //indeces and/or faces
    for (int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    #if 0
    // save into the file
    {
        std::string vb_data = "";
        std::string ib_data = "";

        glm::vec3 color = glm::vec3(0.8f, 0.0f, 0.0f);
        for (auto v : vertices)
        {
            vb_data += std::to_string(v.position.x);
            vb_data += "f, ";
            vb_data += std::to_string(v.position.y);
            vb_data += "f, ";
            vb_data += std::to_string(v.position.z);
            vb_data += "f, ";
            vb_data += std::to_string(v.normal.x);
            vb_data += "f, ";
            vb_data += std::to_string(v.normal.y);
            vb_data += "f, ";
            vb_data += std::to_string(v.normal.z);
            vb_data += "f, ";
            //vb_data += std::to_string(v.texCoord.x);
            vb_data += "0.0";
            vb_data += "f, ";
            //vb_data += std::to_string(v.texCoord.y);
            vb_data += "0.0";
            vb_data += "f, ";
        }
        for (auto i : indices)
        {
            ib_data += std::to_string(i);
            ib_data += ", ";
        }


        std::ofstream out;
        out.open("C:\\Users\\Ondra-PC\\Desktop\\liber_tmp.txt");
        out << vb_data << std::endl;
        out << ib_data << std::endl;
        out.close();

    }
    #endif



    //should be procesing materials, diffuse maps, etc.
    return Mesh(vertices, indices);//textures);

}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	return std::vector<Texture>();
}
