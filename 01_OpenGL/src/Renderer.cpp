#include "Renderer.h"


//linked to GLCall, might have to deal with later
void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << ")" << function << " " << file << ":" << line << std::endl;
		return false;
	}
	return true;
}

Renderer::Renderer(Camera *camera) : m_Camera(camera)
{
	data.Init();
	m_Camera->RegisterObservers(data);
	m_Camera->NotifyObservers();
}

Renderer::Renderer(Camera* camera, Camera* shadowCamera)
{
	data.Init();
	m_Camera = camera;
	m_Camera->RegisterObservers(data);
	m_Camera->NotifyObservers();

	m_shadowCamera = shadowCamera;
	//m_shadowCamera->set_view_from({ 5, -5, 20 });
	//m_shadowCamera->set_view_at({ 0, 0, 0 });
	//m_shadowCamera->Update();
	//m_shadowCamera->RegisterObservers(data);
	//m_shadowCamera->NotifyObservers();
}


void Renderer::DrawObject(Object& object)
{

	Shader* shader = data.shaders["plainColor"];
	shader->Bind();
	object.GetVB()->Bind();
	object.GetVA()->Bind();
	object.GetIB()->Bind();
	shader->SetUniformMat4f("u_Model", object.GetTransform());
	shader->SetUniform4f("u_Color", object.GetColor().x, object.GetColor().y, object.GetColor().z, 1.0f);
	GLCall(glDrawElements(GL_TRIANGLES, object.GetIB()->GetCount(), GL_UNSIGNED_INT, nullptr));
	object.GetVB()->Unbind();
	object.GetVA()->Unbind();
	object.GetIB()->Unbind();
	shader->Unbind();

}

//skyBox
void Renderer::DrawCube(Texture& texture, glm::vec3 position, glm::vec3 scale,
                        float rotation, float xAxes, float yAxes, float zAxes)
{
	glDisable(GL_DEPTH_TEST);

    texture.Bind();

	glm::mat4 transform = glm::translate(glm::mat4(1.0f), { position.x, position.y, position.z })
		* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { xAxes, yAxes, zAxes })
		* glm::scale(glm::mat4(1.0f), { scale.x, scale.y, scale.z });

	data.vb->Bind();
	data.va.Bind();
	data.ib->Bind();
	data.shaders["skyBox"]->Bind();
	//data.shaders["skyBox"]->SetUniformMat4f("u_Projection", m_Camera->GetProjection());
	//data.shaders["skyBox"]->SetUniformMat4f("u_View", m_Camera->GetView());
	data.shaders["skyBox"]->SetUniformMat4f("u_Model", transform);
	GLCall(glDrawElements(GL_TRIANGLES, data.ib->GetCount(), GL_UNSIGNED_INT, nullptr));
	data.vb->Unbind();
	data.va.Unbind();
	data.ib->Unbind();
	data.shaders["skyBox"]->Unbind();
	texture.Unbind();

	glEnable(GL_DEPTH_TEST);
}

void Renderer::DrawCube(Texture& texture, glm::vec3 position, glm::vec3 scale,
	float rotation)
{
	glDisable(GL_DEPTH_TEST);

	texture.Bind();

	glm::mat4 transform = glm::translate(glm::mat4(1.0f), { position.x, position.y, position.z })
		* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 1, 1, 1 })
		* glm::scale(glm::mat4(1.0f), { scale.x, scale.y, scale.z });

	data.vb->Bind();
	data.va.Bind();
	data.ib->Bind();
	data.shaders["skyBox"]->Bind();
	//data.shaders["skyBox"]->SetUniformMat4f("u_Projection", m_Camera->GetProjection());
	//data.shaders["skyBox"]->SetUniformMat4f("u_View", m_Camera->GetView());
	data.shaders["skyBox"]->SetUniformMat4f("u_Model", transform);
	GLCall(glDrawElements(GL_TRIANGLES, data.ib->GetCount(), GL_UNSIGNED_INT, nullptr));
	data.vb->Unbind();
	data.va.Unbind();
	data.ib->Unbind();
	data.shaders["skyBox"]->Unbind();
	texture.Unbind();

	glEnable(GL_DEPTH_TEST);
}

void Renderer::DrawLight(Light& light)
{
    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
    float rotation = 0.0f;

	glm::mat4 transform =
        glm::translate(glm::mat4(1.0f), light.position)
		* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 1.0f, 0.0f, 0.0f })
		* glm::scale(glm::mat4(1.0f), { scale.x, scale.y, scale.z });


    Shader* shader = data.shaders["plainColor"];

    shader->Bind();
	data.vb->Bind();
	data.va.Bind();
	data.ib->Bind();
	shader->Bind();
	//shader->SetUniformMat4f("u_Projection", m_Camera->GetProjection());
	//shader->SetUniformMat4f("u_View", m_Camera->GetView());
	shader->SetUniformMat4f("u_Model", transform);
	shader->SetUniform4f("u_Color", light.color.r, light.color.g, light.color.b, 1.0f);
    GLCall(glDrawElements(GL_TRIANGLES, data.ib->GetCount(), GL_UNSIGNED_INT, nullptr));
	data.vb->Unbind();
	data.va.Unbind();
	data.ib->Unbind();
	shader->Unbind();
}

void Renderer::DrawColor(const glm::vec4& color, glm::vec3 position,
                         float rotation, float xAxes, float yAxes, float zAxes)
{
	glm::vec3 scale = glm::vec3(0.2f, 0.2f, 0.2f);

	glm::mat4 transform =
        glm::translate(glm::mat4(1.0f), { position.x, position.y, position.z })
		* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { xAxes, yAxes, zAxes })
		* glm::scale(glm::mat4(1.0f), { scale.x, scale.y, scale.z });


    Shader* shader = data.shaders["plainColor"];

    shader->Bind();
	data.va.Bind();
	data.ib->Bind();
	shader->Bind();
	//shader->SetUniformMat4f("u_Projection", m_Camera->GetProjection());
	//shader->SetUniformMat4f("u_View", m_Camera->GetView());
	shader->SetUniformMat4f("u_Model", transform);
	shader->SetUniform4f("u_Color", color.r, color.g, color.b, 1.0f);
	GLCall(glDrawElements(GL_TRIANGLES, data.ib->GetCount(), GL_UNSIGNED_INT, nullptr));
	data.vb->Unbind();
	data.va.Unbind();
	data.ib->Unbind();
	shader->Unbind();
}

//model
void Renderer::DrawModel(Texture& diffuse, Texture& specular, Texture& normals,
                         Texture& AO, Texture& roughness, glm::vec3 position,
					     float rotation,
                         Light* lights, float ambientStrength,
                         float Shininess, Model model)
{
	Shader* shader = data.shaders["basic"];
	shader->Bind();

	//glm::vec2 scale = glm::vec2(1.0f, 1.0f);
	glm::vec3 scale = glm::vec3(1.0f);


	glm::mat4 transform =
		glm::translate(glm::mat4(1.0f), { position.x, position.y, position.z })
		* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 1, 0, 0 })
		* glm::scale(glm::mat4(1.0f), { scale.x, scale.y, scale.z });
    //sending all the "lightPos" info into frangment

    for (int i = 0; i < 2; i++)
    {
        std::string u_name = "lights_out[" + std::to_string(i) + "].position";
        shader->SetUniform3fv(u_name, lights[i].position);
        u_name = "lights_out[" + std::to_string(i) + "].color";
        shader->SetUniform3fv(u_name, lights[i].color);
        u_name = "lights_out[" + std::to_string(i) + "].info";
        shader->SetUniform3f(u_name, lights[i].type,
                             glm::cos(glm::radians(lights[i].cutoff)), 0.0f);
        u_name = "lights_out[" + std::to_string(i) + "].lightDir";
        shader->SetUniform3fv(u_name, lights[i].lightDir);

    }
	//shader->SetUniformMat4f("u_Projection", m_Camera->GetProjection());
	//shader->SetUniformMat4f("u_View", m_Camera->GetView());
	shader->SetUniformMat4f("u_Model", transform);
	//shader->SetUniform3f("u_CameraPos", m_Camera->GetPosition().x,
	//	m_Camera->GetPosition().y, m_Camera->GetPosition().z);
	shader->SetUniform1f("u_AmbientStrength", ambientStrength);
	shader->SetUniform1f("u_Shininess", Shininess);


	diffuse.Bind(0);
	shader->SetUniform1i("u_diffuseMap", 0);
	specular.Bind(1);
	shader->SetUniform1i("u_specularMap", 1);
	normals.Bind(2);
	shader->SetUniform1i("u_normalMap", 2);
	AO.Bind(3);
	shader->SetUniform1i("u_AO", 3);
	roughness.Bind(4);
	shader->SetUniform1i("u_roughness", 4);
    shader->Bind();
    for(int i = 0; i < model.meshes.size(); i++)
	{
		
	
        model.meshes[i].vb->Bind();
		model.meshes[i].va->Bind();
		model.meshes[i].ib->Bind();

		GLCall(glDrawElements(GL_TRIANGLES, model.meshes[i].ib->GetCount(), GL_UNSIGNED_INT, nullptr));
		model.meshes[i].vb->Unbind();
		model.meshes[i].va->Unbind();
		model.meshes[i].ib->Unbind();

	}
	shader->Unbind();
}

// completeModel
void Renderer::DrawModel(glm::vec3 position, float rotation, float scale, Light* lights, PG2::Model model)
{
	Shader* shader = data.shaders["pg2"];
	shader->Bind();

	//glm::vec2 scale = glm::vec2(1.0f, 1.0f);
	glm::vec3 scale_ = glm::vec3(scale, scale, scale);


	glm::mat4 transform =
		glm::translate(glm::mat4(1.0f), { position.x, position.y, position.z })
		* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), {0, 1, 1})
		* glm::scale(glm::mat4(1.0f), { scale_.x, scale_.y, scale_.z });

	//Texture* tex = new Texture(1024);
	//GLuint depthMapFBO = tex->fbo_shadow_map_;
	//
	//glBindFramebuffer(GL_FRAMEBUFFER, tex->fbo_shadow_map_);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tex->m_RendererID, 0);
	//glDrawBuffer(GL_NONE);
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//
	//glViewport(0, 0, 1024.0f, 1024.0f);
	//glBindFramebuffer(GL_FRAMEBUFFER, tex->fbo_shadow_map_);
	//glClear(GL_DEPTH_BUFFER_BIT);

	//glViewport(0, 0, 960.0f, 540.0f);

	//glm::mat4 transform = glm::translate(glm::mat4(1.0f), { position.x, position.y, position.z })
	//	* glm::scale(glm::mat4(1.0f), { scale_.x, scale_.y, scale_.z });

	//sending all the "lightPos" info into frangment

	//for (int i = 0; i < 2; i++)
	//{
	//	std::string u_name = "lights_out[" + std::to_string(i) + "].position";
	//	shader->SetUniform3fv(u_name, lights[i].position);
	//	u_name = "lights_out[" + std::to_string(i) + "].color";
	//	shader->SetUniform3fv(u_name, lights[i].color);
	//	u_name = "lights_out[" + std::to_string(i) + "].info";
	//	shader->SetUniform3f(u_name, lights[i].type,
	//		glm::cos(glm::radians(lights[i].cutoff)), 0.0f);
	//	u_name = "lights_out[" + std::to_string(i) + "].lightDir";
	//	shader->SetUniform3fv(u_name, lights[i].lightDir);
	//
	//}

	for (int i = 0; i < 2; i++)
	{
		std::string u_name = "lights_out[" + std::to_string(i) + "].position";
		shader->SetUniform3f(u_name, lights[i].position.x, lights[i].position.y, lights[i].position.z);
		u_name = "lights_out[" + std::to_string(i) + "].color";
		shader->SetUniform3fv(u_name, lights[i].color);
		u_name = "lights_out[" + std::to_string(i) + "].info";
		shader->SetUniform3f(u_name, lights[i].type,
			glm::cos(glm::radians(lights[i].cutoff)), 0.0f);
		u_name = "lights_out[" + std::to_string(i) + "].lightDir";
		shader->SetUniform3fv(u_name, lights[i].lightDir);

	}

	for (int i = 0; i < model.materials.size(); i++)
	{
		std::string u_name = "materials[" + std::to_string(i) + "].Ns";
		shader->SetUniform1f(u_name, model.materials[i].Ns);
		u_name = "materials[" + std::to_string(i) + "].Ni";
		shader->SetUniform1f(u_name, model.materials[i].Ni);
		u_name = "materials[" + std::to_string(i) + "].Ka";
		shader->SetUniform3f(u_name, model.materials[i].Ka);
		u_name = "materials[" + std::to_string(i) + "].Kd";
		shader->SetUniform3f(u_name, model.materials[i].Kd);
		u_name = "materials[" + std::to_string(i) + "].Ks";
		shader->SetUniform3f(u_name, model.materials[i].Ks);
		u_name = "materials[" + std::to_string(i) + "].Ke";
		shader->SetUniform3f(u_name, model.materials[i].Ke);
	}


	//shader->SetUniformMat4f("u_Projection", m_Camera->GetProjection());
	//shader->SetUniformMat4f("u_View", m_Camera->GetView());
	shader->SetUniformMat4f("u_Model", transform);
	shader->SetUniformMat4f("u_Projection", m_Camera->ProjectionMatrix);
	shader->SetUniformMat4f("u_View", m_Camera->ViewMatrix);
	shader->SetUniformMat4f("u_lightView", m_shadowCamera->ViewMatrix);
	shader->SetUniformMat4f("u_lightProjection", m_shadowCamera->ProjectionMatrix);
	
	//shader->SetUniform3f("u_CameraPos", m_Camera->GetPosition().x, m_Camera->GetPosition().y, m_Camera->GetPosition().z);
	

	for (int i = 0; i < model.meshes.size(); i++)
	{
		model.textures["blank"]->Bind(0);
		shader->SetUniform1i("u_diffuseMap", 0);

		shadowTexture->Bind(3);
		shader->SetUniform1i("u_shadowMap", 3);

		if (model.meshes[i].textures.size() > 0)
		{
			for (int t = 0; t < model.meshes[i].textures.size(); t++)
			{
				std::string tex_name = model.meshes[i].textures[t].name;

				if (model.meshes[i].textures[t].type == "texture_diffuse")
				{
					model.textures[tex_name]->Bind(0);
					shader->SetUniform1i("u_diffuseMap", 0);
				}
				if (model.meshes[i].textures[t].type == "texture_normal")
				{
					model.textures[tex_name]->Bind(1);
					shader->SetUniform1i("u_normalMap", 1);
				}
				if (model.meshes[i].textures[t].type == "texture_rma")
				{
					model.textures[tex_name]->Bind(2);
					shader->SetUniform1i("u_rmaMap", 2);
				}

			}

		}

		model.meshes[i].vb->Bind();
		model.meshes[i].va->Bind();
		model.meshes[i].ib->Bind();

		GLCall(glDrawElements(GL_TRIANGLES, model.meshes[i].ib->GetCount(), GL_UNSIGNED_INT, nullptr));
		model.meshes[i].vb->Unbind();
		model.meshes[i].va->Unbind();
		model.meshes[i].ib->Unbind();

	}


	shader->Unbind();
}

void Renderer::DrawModel1(glm::vec3 position, float rotation, float scale, Light* lights, PG2::Model model, float t)
{
	Shader* shader = data.shaders["pg2"];
	shader->Bind();

	//glm::vec2 scale = glm::vec2(1.0f, 1.0f);
	glm::vec3 scale_ = glm::vec3(scale, scale, scale);


	glm::mat4 transform =
		glm::translate(glm::mat4(1.0f), { position.x, position.y, position.z })
		* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0, 1, 1 })
		* glm::scale(glm::mat4(1.0f), { scale_.x, scale_.y, scale_.z });

	//transform = glm::scale(glm::mat4(1.0f), { scale_.x, scale_.y, scale_.z });


	//Texture* tex = new Texture(1024);
	//GLuint depthMapFBO = tex->fbo_shadow_map_;
	//
	//glBindFramebuffer(GL_FRAMEBUFFER, tex->fbo_shadow_map_);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tex->m_RendererID, 0);
	//glDrawBuffer(GL_NONE);
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//
	//glViewport(0, 0, 1024.0f, 1024.0f);
	//glBindFramebuffer(GL_FRAMEBUFFER, tex->fbo_shadow_map_);
	//glClear(GL_DEPTH_BUFFER_BIT);

	//glViewport(0, 0, 960.0f, 540.0f);

	//glm::mat4 transform = glm::translate(glm::mat4(1.0f), { position.x, position.y, position.z })
	//	* glm::scale(glm::mat4(1.0f), { scale_.x, scale_.y, scale_.z });

	//sending all the "lightPos" info into frangment

	//for (int i = 0; i < 2; i++)
	//{
	//	std::string u_name = "lights_out[" + std::to_string(i) + "].position";
	//	shader->SetUniform3fv(u_name, lights[i].position);
	//	u_name = "lights_out[" + std::to_string(i) + "].color";
	//	shader->SetUniform3fv(u_name, lights[i].color);
	//	u_name = "lights_out[" + std::to_string(i) + "].info";
	//	shader->SetUniform3f(u_name, lights[i].type,
	//		glm::cos(glm::radians(lights[i].cutoff)), 0.0f);
	//	u_name = "lights_out[" + std::to_string(i) + "].lightDir";
	//	shader->SetUniform3fv(u_name, lights[i].lightDir);
	//
	//}

	//glPointParameteri(GL_POINT_SPRITE_COORD_ORIGIN, GL_UPPER_LEFT);

	for (int i = 0; i < 2; i++)
	{
		std::string u_name = "lights_out[" + std::to_string(i) + "].position";
		shader->SetUniform3f(u_name, lights[i].position.x, lights[i].position.y, lights[i].position.z);
		u_name = "lights_out[" + std::to_string(i) + "].color";
		shader->SetUniform3fv(u_name, lights[i].color);
		u_name = "lights_out[" + std::to_string(i) + "].info";
		shader->SetUniform3f(u_name, lights[i].type,
			glm::cos(glm::radians(lights[i].cutoff)), 0.0f);
		u_name = "lights_out[" + std::to_string(i) + "].lightDir";
		shader->SetUniform3fv(u_name, lights[i].lightDir);

	}

	for (int i = 0; i < model.materials.size(); i++)
	{
		std::string u_name = "materials[" + std::to_string(i) + "].Ns";
		shader->SetUniform1f(u_name, model.materials[i].Ns);
		u_name = "materials[" + std::to_string(i) + "].Ni";
		shader->SetUniform1f(u_name, model.materials[i].Ni);
		u_name = "materials[" + std::to_string(i) + "].Ka";
		shader->SetUniform3f(u_name, model.materials[i].Ka);
		u_name = "materials[" + std::to_string(i) + "].Kd";
		shader->SetUniform3f(u_name, model.materials[i].Kd);
		u_name = "materials[" + std::to_string(i) + "].Ks";
		shader->SetUniform3f(u_name, model.materials[i].Ks);
		u_name = "materials[" + std::to_string(i) + "].Ke";
		shader->SetUniform3f(u_name, model.materials[i].Ke);
	}

	//m_shadowCamera->Update();
	//m_Camera->Update();
	//shader->SetUniformMat4f("u_Projection", m_Camera->GetProjection());
	//shader->SetUniformMat4f("u_View", m_Camera->GetView());
	// draw															from        to
	shader->SetUniformMat4f("u_Model", transform);
	shader->SetUniformMat4f("u_View", m_Camera->get_view());
	shader->SetUniformMat4f("u_Projection", m_Camera->get_projection());
		//shader->SetUniform3f("u_CameraPos", m_Camera->GetPosition().x, m_Camera->GetPosition().y, m_Camera->GetPosition().z);
	shader->SetUniformMat4f("u_lightView", m_shadowCamera->get_view());
	shader->SetUniformMat4f("u_lightProjection", m_shadowCamera->get_projection());
	shader->SetUniform3f("u_CameraPos", m_Camera->m_Position.x, m_Camera->m_Position.y, m_Camera->m_Position.z);

	

	glm::mat4 mvp = m_Camera->get_projection() * m_Camera->get_view() * transform;
	glm::mat4 mdl = transform;
	glm::mat4 mlp = m_shadowCamera->get_projection() * m_shadowCamera->get_view() * transform;

	shader->SetUniformMat4f("MVP", mvp);
	shader->SetUniformMat4f("MDL", mdl);
	shader->SetUniformMat4f("MLP", mlp);


	// MVP
	// MDL
	// MLP


	for (int i = 0; i < model.meshes.size(); i++)
	{
		model.textures["blank"]->Bind(0);
		shader->SetUniform1i("u_diffuseMap", 0);
		
		// shadow
		shadowTexture->Bind(3);
		shader->SetUniform1i("u_shadowMap", 3);

		if (model.meshes[i].textures.size() > 0)
		{
			for (int t = 0; t < model.meshes[i].textures.size(); t++)
			{
				std::string tex_name = model.meshes[i].textures[t].name;

				if (model.meshes[i].textures[t].type == "texture_diffuse")
				{
					model.textures[tex_name]->Bind(0);
					shader->SetUniform1i("u_diffuseMap", 0);
				}
				if (model.meshes[i].textures[t].type == "texture_normal")
				{
					model.textures[tex_name]->Bind(1);
					shader->SetUniform1i("u_normalMap", 1);
				}
				if (model.meshes[i].textures[t].type == "texture_rma")
				{
					model.textures[tex_name]->Bind(2);
					shader->SetUniform1i("u_rmaMap", 2);
				}

			}

		}

		model.meshes[i].vb->Bind();
		model.meshes[i].va->Bind();
		model.meshes[i].ib->Bind();

		GLCall(glDrawElements(GL_TRIANGLES, model.meshes[i].ib->GetCount(), GL_UNSIGNED_INT, nullptr));
		model.meshes[i].vb->Unbind();
		model.meshes[i].va->Unbind();
		model.meshes[i].ib->Unbind();

	}


	shader->Unbind();
}

void Renderer::DrawShadow(glm::vec3 position, float rotation, float scale, Light* lights, PG2::Model model)
{
	//m_shadowCamera->set_view_from({ 30, 20, 50 });
	//m_shadowCamera->set_view_at({ 0, 0, 0 });


	//m_Camera->set_view_from({ 30, 20, 50 });
	//m_shadowCamera->set_view_from({ 30, 20, 50 });

	//glDisable(GL_DEPTH_TEST);
	glEnable(GL_DEPTH_TEST);

	glCullFace(GL_FRONT);
	
	glClipControl(GL_LOWER_LEFT, GL_NEGATIVE_ONE_TO_ONE);

	Shader* shader = data.shaders["shadow"];
	shader->Bind();

	//glm::vec2 scale = glm::vec2(1.0f, 1.0f);
	glm::vec3 scale_ = glm::vec3(scale, scale, scale);


	glm::mat4 transform =
		glm::translate(glm::mat4(1.0f), { position.x, position.y, position.z })
		* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0, 1, 1 })
		* glm::scale(glm::mat4(1.0f), { scale_.x, scale_.y, scale_.z });

	//transform = glm::scale(glm::mat4(1.0f), { scale_.x, scale_.y, scale_.z });

	//Camera* cam = new Camera(960.0f, 540.0f, glm::radians(45.0f), lights[0].position, {5, 5, 5});

	// first pass
	
	
	//glBindFramebuffer(GL_FRAMEBUFFER, tex->fbo_shadow_map_);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tex->m_RendererID, 0);
	//glDrawBuffer(GL_NONE);
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	
	//glBindFramebuffer(GL_FRAMEBUFFER, tex->fbo_shadow_map_);
	//glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	//glClear(GL_DEPTH_BUFFER_BIT);
	
	// draw															from        to
	
	//m_shadowCamera->Update();
	
	// ortho test
	glm::mat4 lightProj__ = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, 1.0f, 50.0f);
	glm::mat4 lightView__ = glm::lookAt(m_shadowCamera->view_from_, { 0, 0, 0 }, {0, 0, 1});


	shader->Bind();
	shader->SetUniformMat4f("u_lightProjection", lightProj__);
	shader->SetUniformMat4f("u_lightView", lightView__);

	glm::mat4 mlp = glm::mat4(m_shadowCamera->get_projection() * m_shadowCamera->get_view());
	glm::mat4 mlp_ = mlp * transform;

	shader->SetUniformMat4f("MLP", mlp_);
	//s.second->SetUniform3f("u_CameraPos", position.x, position.y, position.z);
	shader->SetUniformMat4f("u_Model", transform);
	
	//glViewport(0, 0, 2048.0f, 2048.0f);
	GLuint depthMapFBO = shadowTexture->fbo_shadow_map_;
	//shadowTexture->Bind(3);
	
	//glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	//glBindFramebuffer(GL_FRAMEBUFFER, shadowTexture->fbo_shadow_map_);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowTexture->m_RendererID, 0);
	//glDrawBuffer(GL_NONE);
	//glReadBuffer(GL_NONE);
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, 1024.0f, 1024.0f);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowTexture->fbo_shadow_map_);
	glClear(GL_DEPTH_BUFFER_BIT);
	
	for (int i = 0; i < model.meshes.size(); i++)
	{
		model.meshes[i].vb->Bind();
		model.meshes[i].va->Bind();
		model.meshes[i].ib->Bind();
		GLCall(glDrawElements(GL_TRIANGLES, model.meshes[i].ib->GetCount(), GL_UNSIGNED_INT, nullptr));
		model.meshes[i].vb->Unbind();
		model.meshes[i].va->Unbind();
		model.meshes[i].ib->Unbind();

	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// second
	glViewport(0, 0, 960.0f, 540.0f);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // tohle 
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT); // tohle 

	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);

	DrawModel1(position, rotation, scale, lights, model, 1.0f);
	

}


// sphericalMapEnviroment
void Renderer::DrawModel(glm::vec3 position, float rotation, float scale, PG2::Model model)
{
	glDisable(GL_DEPTH_TEST);

	Shader* shader = data.shaders["pg2env"];
	shader->Bind();

	//glm::vec2 scale = glm::vec2(1.0f, 1.0f);
	glm::vec3 scale_ = glm::vec3(scale, scale, scale);


	glm::mat4 transform =
		glm::translate(glm::mat4(1.0f), { position.x, position.y, position.z })
		* glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), { 0, 1, 1 })
		* glm::scale(glm::mat4(1.0f), { scale_.x, scale_.y, scale_.z });
	//glm::mat4 transform = glm::translate(glm::mat4(1.0f), { position.x, position.y, position.z })
	//	* glm::scale(glm::mat4(1.0f), { scale_.x, scale_.y, scale_.z });

	//sending all the "lightPos" info into frangment
	
	/*
	for (int i = 0; i < 2; i++)
	{
		std::string u_name = "lights_out[" + std::to_string(i) + "].position";
		shader->SetUniform3fv(u_name, lights[i].position);
		u_name = "lights_out[" + std::to_string(i) + "].color";
		shader->SetUniform3fv(u_name, lights[i].color);
		u_name = "lights_out[" + std::to_string(i) + "].info";
		shader->SetUniform3f(u_name, lights[i].type,
			glm::cos(glm::radians(lights[i].cutoff)), 0.0f);
		u_name = "lights_out[" + std::to_string(i) + "].lightDir";
		shader->SetUniform3fv(u_name, lights[i].lightDir);

	}
	*/

	for (int i = 0; i < model.materials.size(); i++)
	{
		std::string u_name = "materials[" + std::to_string(i) + "].Ns";
		shader->SetUniform1f(u_name, model.materials[i].Ns);
		u_name = "materials[" + std::to_string(i) + "].Ni";
		shader->SetUniform1f(u_name, model.materials[i].Ni);
		u_name = "materials[" + std::to_string(i) + "].Ka";
		shader->SetUniform3f(u_name, model.materials[i].Ka);
		u_name = "materials[" + std::to_string(i) + "].Kd";
		shader->SetUniform3f(u_name, model.materials[i].Kd);
		u_name = "materials[" + std::to_string(i) + "].Ks";
		shader->SetUniform3f(u_name, model.materials[i].Ks);
		u_name = "materials[" + std::to_string(i) + "].Ke";
		shader->SetUniform3f(u_name, model.materials[i].Ke);
	}


	//shader->SetUniformMat4f("u_Projection", m_Camera->GetProjection());
	//shader->SetUniformMat4f("u_View", m_Camera->GetView());
	shader->SetUniformMat4f("u_Model", transform);
	//shader->SetUniform3f("u_CameraPos", m_Camera->GetPosition().x, m_Camera->GetPosition().y, m_Camera->GetPosition().z);


	for (int i = 0; i < model.meshes.size(); i++)
	{
		model.textures["blank"]->Bind(0);
		shader->SetUniform1i("u_diffuseMap", 0);

		if (model.meshes[i].textures.size() > 0)
		{
			for (int t = 0; t < model.meshes[i].textures.size(); t++)
			{
				std::string tex_name = model.meshes[i].textures[t].name;

				if (model.meshes[i].textures[t].type == "texture_diffuse")
				{
					model.textures[tex_name]->Bind(0);
					shader->SetUniform1i("u_diffuseMap", 0);
				}
				if (model.meshes[i].textures[t].type == "texture_normal")
				{
					model.textures[tex_name]->Bind(1);
					shader->SetUniform1i("u_normalMap", 1);
				}
				if (model.meshes[i].textures[t].type == "texture_rma")
				{
					model.textures[tex_name]->Bind(2);
					shader->SetUniform1i("u_rmaMap", 2);
				}

			}

		}

		model.meshes[i].vb->Bind();
		model.meshes[i].va->Bind();
		model.meshes[i].ib->Bind();

		GLCall(glDrawElements(GL_TRIANGLES, model.meshes[i].ib->GetCount(), GL_UNSIGNED_INT, nullptr));
		model.meshes[i].vb->Unbind();
		model.meshes[i].va->Unbind();
		model.meshes[i].ib->Unbind();

	}
	shader->Unbind();

	glEnable(GL_DEPTH_TEST);
}



// scrap
void Renderer::DrawMap(const RandomMap& map, glm::vec3 position, Light* lights, bool drawNormals)
{

    //glStencilFunc(GL_ALWAYS, 1, 0xFF);

    Shader* shader = data.shaders["procedural"];

    float rotation = 90.0f;
	glm::vec3 scale = { map.scale.x, map.scale.y , map.scale.z };

	glm::mat4 transform =
        glm::translate(glm::mat4(1.0f), { position.x, position.y, position.z })
		* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 1, 0, 0 })
		* glm::scale(glm::mat4(1.0f), { scale.x, scale.y, scale.z });

    float Shininess = 64.0f;
    float ambientStrength = 0.1f;

    map.vb->Bind();
	map.va->Bind();
	map.ib->Bind();

    shader->Bind();

	// sending in light data
	for (int i = 0; i < 2; i++)
	{
		std::string u_name = "lights[" + std::to_string(i) + "].position";
		shader->SetUniform3fv(u_name, lights[i].position);
		u_name = "lights[" + std::to_string(i) + "].color";
		shader->SetUniform3fv(u_name, lights[i].color);
		u_name = "lights[" + std::to_string(i) + "].info";
		shader->SetUniform3f(u_name, lights[i].type,
			glm::cos(glm::radians(lights[i].cutoff)), 0.0f);
		u_name = "lights[" + std::to_string(i) + "].lightDir";
		shader->SetUniform3fv(u_name, lights[i].lightDir);

	}

    //shader->SetUniform3f("u_CameraPos", m_Camera->GetPosition().x,
	//	m_Camera->GetPosition().y, m_Camera->GetPosition().z);
	shader->SetUniform1f("u_AmbientStrength", ambientStrength);
	shader->SetUniform1f("u_Shininess", Shininess);
    //shader->SetUniformMat4f("u_Projection", m_Camera->GetProjection());
	//shader->SetUniformMat4f("u_View", m_Camera->GetView());
	shader->SetUniformMat4f("u_Model", transform);
	shader->SetUniform4f("u_Color", 0.1f, 0.5f, 0.05f, 1.0f);
	GLCall(glDrawElements(GL_TRIANGLES, map.ib->GetCount(), GL_UNSIGNED_INT, nullptr));
	map.vb->Unbind();
	map.va->Unbind();
	map.ib->Unbind();
	shader->Unbind();

    // draw the normals!
    if (drawNormals)
	{
		for (int i = 0; i < map.vertices.size(); i++)
		{
		    glm::vec3 _position = glm::vec3(map.vertices[i].position.x * map.scale.x, map.vertices[i].position.y * map.scale.y, map.vertices[i].position.z *	map.scale.z);

		    DrawLine(_position,
		             _position + map.vertices[i].normal);
		}
	}
}

void Renderer::DrawLine(glm::vec3 p1, glm::vec3 p2)
{

	float line_vertex[] =
	{
		p1.x, p1.y, p1.z, 1.0f, 1.0f, 1.0f,
		p2.x, p2.y, p2.z, 1.0f, 1.0f, 1.0f,
	};

	unsigned int indices[] =
	{
		0, 1,
	};

	glm::mat4 transform = glm::mat4(1.0f);

	VertexBuffer* vb = new VertexBuffer(line_vertex, sizeof(line_vertex));
	IndexBuffer* ib = new IndexBuffer(indices, 2);
	VertexArray* va = new VertexArray();
	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(3);
	va->AddBuffer(*vb, layout);

	vb->Bind();
	va->Bind();
	ib->Bind();
	data.shaders["plainColor"]->Bind();
	//data.shaders["plainColor"]->SetUniformMat4f("u_Projection", m_Camera->GetProjection());
	//data.shaders["plainColor"]->SetUniformMat4f("u_View", m_Camera->GetView());
	data.shaders["plainColor"]->SetUniformMat4f("u_Model", transform);
	data.shaders["plainColor"]->SetUniform4f("u_Color", 0.1f, 0.9f, 0.05f, 1.0f);
	glVertexPointer(2, GL_FLOAT, 0, line_vertex);
	glLineWidth(5);
	GLCall(glDrawElements(GL_LINES, ib->GetCount(), GL_UNSIGNED_INT, nullptr));

	delete vb;
	delete ib;
	delete va;

}

void Renderer::DrawChar(FontSheet& font, char32_t character, glm::vec3 position, glm::vec3 color)
{

	glDisable(GL_DEPTH_TEST);
    font.texture->Bind();
    data.shaders["font"]->Bind();

    float rotation = 0.0f;
    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

	glm::mat4 transform = glm::translate(glm::mat4(1.0f), { position.x, position.y, position.z })
		* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
		* glm::scale(glm::mat4(1.0f), { scale.x, scale.y, scale.z });


	glm::vec2* vertex_texCoords = new glm::vec2[4];
    vertex_texCoords = font.get_coords(character);

	glm::vec3* vertex_positions = new glm::vec3[4];
	vertex_positions[0] = { -0.5f, -0.5f, 0.0f };
	vertex_positions[1] = { 0.5f, -0.5f, 0.0f };
	vertex_positions[2] = { 0.5f,  0.5f, 0.0f };
	vertex_positions[3] = { -0.5f,  0.5f, 0.0f };

    float vertex_info[(3 + 2) * 4];
    {
        for(int i = 0; i < 4; i++)
        {
            vertex_info[(i * 5) + 0] = vertex_positions[i].x;
            vertex_info[(i * 5) + 1] = vertex_positions[i].y;
            vertex_info[(i * 5) + 2] = vertex_positions[i].z;
            vertex_info[(i * 5) + 3] = vertex_texCoords[i].x;
            vertex_info[(i * 5) + 4] = vertex_texCoords[i].y;
        }
    }

    unsigned int indices[] =
	{
		0, 1, 2,
        2, 3, 0
	};

	delete[] vertex_positions;

	data.shaders["font"]->SetUniform4f("u_Color", color.x, color.y, color.z, 1.0f);
    data.shaders["font"]->SetUniformMat4f("u_Model", transform);

	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(2);
    VertexBuffer* vb = new VertexBuffer(vertex_info, sizeof(vertex_info));
    IndexBuffer* ib = new IndexBuffer(indices, 6);
    VertexArray* va = new VertexArray();

    va->AddBuffer(*vb, layout);
    vb->Bind();
    va->Bind();
    ib->Bind();
    GLCall(glDrawElements(GL_TRIANGLES, ib->GetCount(), GL_UNSIGNED_INT, nullptr));

    // cleanup
    vb->Unbind();
    va->Unbind();
    ib->Unbind();
	font.texture->Unbind();
    data.shaders["font"]->Unbind();
	delete vb;
    delete ib;
    delete va;

    glEnable(GL_DEPTH_TEST);

}

void Renderer::DrawFont(FontSheet& font, std::string text, glm::vec3 position, glm::vec3 color)
{
    float font_render_step = 0.0f;

    for (char32_t c : text)
    {
        DrawChar(font, c, { position.x + font_render_step, position.y, position.z }, color);
        font_render_step += 0.6f;
    }
}

void Renderer::Clear() const
{
	GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
}
