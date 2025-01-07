#pragma once
#include "GL/glew.h"

#include "glm/mat4x4.hpp"

#include <memory>
#include <string>

#include "RenderData.h"

#include "Camera.h"
#include "ParticleSystem.h"

#include "SubTexture.h"
#include <unordered_map>
#include "Model.h"
#include "Enemy.h"

#include "RandomMap.h"

#include "Light.h"

#include "FontSheet.h"

#include "CompleteModel.h"

class Renderer
{
private:
	Camera* m_Camera;
	RenderData data;
	Texture* shadowTexture = new Texture(1024.0f);

	Texture* blank = new Texture("src/res/textures/Blank.png");
	Texture* grid = new Texture("src/res/textures/grid.png");
public:

	Camera* m_shadowCamera;
	Renderer(Camera* camera);
	Renderer(Camera* camera, Camera* shadowCamera);

    // draw a light component
	void DrawObject(Object& object);
	void DrawLight(Light& light);
    // skyBox
	void DrawCube(Texture& texture, glm::vec3 position, glm::vec3 scale,
                  float rotation, float xAxes, float yAxes, float zAxes);
	void DrawCube(Texture& texture, glm::vec3 position, glm::vec3 scale,
		float rotation);
    // plain color / light
	void DrawColor(const glm::vec4& color, glm::vec3 position,
                   float rotation, float xAxes, float yAxes, float zAxes);
    // model
	void DrawModel(Texture& diffuse, Texture& specular, Texture& normals,
					Texture& AO, Texture& roughness, glm::vec3 position,
		            float rotation,
					Light* lights, float ambientStrength,
					float Shininess, Model model);
	// completeModel
	void DrawModel(glm::vec3 position, float rotation, float scale, Light* lights, PG2::Model model);
	// spherical map
	void DrawModel(glm::vec3 position, float rotation, float scale, PG2::Model model);
	void DrawModel1(glm::vec3 position, float rotation, float scale, Light* lights, PG2::Model model, float t);
    // procedural map
    void DrawMap(const RandomMap& map, glm::vec3 position, Light* lights, bool drawNormals);
	void DrawLine(glm::vec3 p1, glm::vec3 p2);
    // draw font
    void DrawChar(FontSheet& font, char32_t character, glm::vec3 position, glm::vec3 color);
    void DrawFont(FontSheet& font, std::string text, glm::vec3 position, glm::vec3 color);

	void DrawShadow(glm::vec3 position, float rotation, float scale, Light* lights, PG2::Model model);

	void Clear() const;
};
