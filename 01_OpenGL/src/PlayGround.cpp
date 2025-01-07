#include "PlayGround.h"
#include "Enemy.h"

#define SHADOWCAMERAHEIGHT 30

void PlayGround::OnAttach()
{
	EventHandler::camera = camera;
	//EventHandler::mouseRay = worldRay;

	glfwSetCursorPosCallback(window, [](GLFWwindow* window, double mouseXPos,
		double mouseYPos)
		-> void {EventHandler::cursor_pos_callback(window, mouseXPos, mouseYPos); });
	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode,
		int action, int mods)
		-> void {EventHandler::key_callback(window, key, scancode, action, mods); });

	glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button,
		int action, int mode)
		-> void {EventHandler::mouse_button_callback(window, button, action, mode); });

	lightPositions[0] = glm::vec3(5, 5, 8);
	lightPositions[1] = glm::vec3(5, 5, -10);

	Light l0;
	l0.color = glm::vec3(1.0f);
	l0.position = glm::vec3(0.1f, 0.1, 30);
	l0.type = LightType::Point;
	l0.lightDir = glm::vec3(0.0f, 0.0f, -1.0f);
	l0.cutoff = 12.0f;
	lights[0] = l0;

	Light l1;
	l1.color = glm::vec3(0.5f);
	l1.position = glm::vec3(6, -20, 10.5f);
	l1.type = LightType::Point;
	l1.lightDir = glm::vec3(0.0f, 0.0f, 1.0f);
	l1.cutoff = 12.0f;
	lights[1] = l1;
	//in future ure gonna just pass the "ojb. folder" -> that folder WILL have to include
	//texture files in correct form (AO.png, .. )
	
	//model_teren = new Model("src\\res\\models\\teren\\teren__.obj", 1);
	//model = new Model("src/res/models/backpack/backpack.obj", 0);
	//model = new Model("src/res/models/6887_allied_avenger/6887_allied_avenger_gi2.obj", 0);


	m = new PG2::Model("src/res/models/6887_allied_avenger/6887_allied_avenger_gi2.obj");
	
	//m = new PG2::Model("src/res/models/piece_02/piece_02.obj");

	//m = new PG2::Model("src/res/models/backpack/backpack.obj");

	//m = new PG2::Model("src/res/models/shadowtest/shadowtest.obj");

	skySphere = new PG2::Model("src/res/models/sphere/geosphere.obj");


	//m = new PG2::Model("src/res/models/backpack/backpack.obj");
	//std::cout << "t";

	//model_zidle = new Model("src\\res\\models\\zidle\\zidle__.obj", 1);
	//model_stul = new Model("src\\res\\models\\stul\\stul__.obj", 1);
	//model_kniha = new Model("src\\res\\models\\kniha\\kniha__.obj", 1);
	//model_vaza = new Model("src\\res\\models\\vaza\\vaza__.obj", 1);
	//model_podlaha = new Model("src\\res\\models\\podlaha\\podlaha__.obj", 1);
	//model_sipka = new Model("src\\res\\models\\sipka\\sipka.obj", 1);

	// init map here..
	map->Init();

	// test

	{
		float cZoom = 15.0f;
		//float cZoom = 35.0f;
		glm::vec3 p1 = glm::vec3(cZoom, 0.0f, 0.0f);
		glm::vec3 p2 = glm::vec3(0.0f, cZoom, 0.0f);
		glm::vec3 p3 = glm::vec3(-cZoom, 0.0f, 0.0f);
		glm::vec3 p4 = glm::vec3(cZoom, -cZoom * 2, 0.0f);
		glm::mat4x3 bezier = {
			p1.x, p1.y, p1.z,
			p2.x, p2.y, p2.z,
			p3.x, p3.y, p3.z,
			p4.x, p4.y, p4.z
		};
		Object* o = new Enemy(bezier);
		OM->Add(*o);
	}

	{
		float cZoom = 1.0f;
		//float cZoom = 5.0f;
		glm::vec3 p1 = glm::vec3(-15.1f, -15.1f, SHADOWCAMERAHEIGHT);
		glm::vec3 p2 = glm::vec3(-12.1f, -15.1f, SHADOWCAMERAHEIGHT);
		glm::vec3 p3 = glm::vec3( 12.1f,  15.1f, SHADOWCAMERAHEIGHT);
		glm::vec3 p4 = glm::vec3( 15.1f,  15.1f, SHADOWCAMERAHEIGHT);
		glm::mat4x3 bezier = {
			p1.x, p1.y, p1.z,
			p2.x, p2.y, p2.z,
			p3.x, p3.y, p3.z,
			p4.x, p4.y, p4.z
		};
		Object* o = new Enemy(bezier);
		OM->Add(*o);
	}

	/*
	for (int i = 0; i < 5; i++)
	{
		if (Random::Float() > 0.5f)
		{
			glm::vec3 p1 = glm::vec3(Random::Float() * 10, Random::Float() * 10 + 5, Random::Float() * 10);
			glm::vec3 p2 = glm::vec3(Random::Float() * 10, Random::Float() * 10 + 5, Random::Float() * 10);
			glm::vec3 p3 = glm::vec3(Random::Float() * 10, Random::Float() * 10 + 5, Random::Float() * 10);
			glm::vec3 p4 = glm::vec3(Random::Float() * 10, Random::Float() * 10 + 5, Random::Float() * 10);
			glm::mat4x3 bezier = {
				p1.x, p1.y, p1.z,
				p2.x, p2.y, p2.z,
				p3.x, p3.y, p3.z,
				p4.x, p4.y, p4.z
			};
			Object* o = new Enemy(bezier);
			OM->Add(*o);
		}
		else
		{
			glm::vec3 p1 = glm::vec3(Random::Float() * 10, Random::Float() * 10 + 5, Random::Float() * 10);
			glm::vec3 p2 = glm::vec3(Random::Float() * 10, Random::Float() * 10 + 5, Random::Float() * 10);
			glm::vec3 p3 = glm::vec3(Random::Float() * 10, Random::Float() * 10 + 5, Random::Float() * 10);
			glm::vec3 p4 = glm::vec3(Random::Float() * 10, Random::Float() * 10 + 5, Random::Float() * 10);
			glm::mat4x3 bezier = {
				p1.x, p1.y, p1.z,
				p2.x, p2.y, p2.z,
				p3.x, p3.y, p3.z,
				p4.x, p4.y, p4.z
			};
			Object* o = new Ally(bezier);
			OM->Add(*o);
		}
	}
	*/

	EventHandler::camera = camera;
	EventHandler::object_manager = OM;

	loader->init();

}

void PlayGround::OnDetach()
{
    delete camera;
    delete renderer;
    delete alonso;
    delete tex;
    delete skyBox;
    // delete diffuse;
    // delete specular;
    // delete normal;
    // delete ao;
    // delete roughness;
    //delete anim;
    delete grass;
    //delete animation;
    //delete model;
}

void PlayGround::OnUpdate()
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera->MoveForward();
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera->MoveLeft();
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera->MoveBackward();
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera->MoveRight();
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera->MoveUp();
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera->MoveDown();



	//animation->OnUpdate();
	//rotation += 1;

	t += 0.002f * direction;
	if (t > 1)
		direction = -1;
	if (t < 0)
		direction = 1;


	glm::vec4 matOne = glm::vec4(t * t * t, t * t, t, 1);

	glm::mat4 matTwo = {
		-1,  3, -3, 1,
		 3, -6,  3, 0,
		-3,  3,  0, 0,
		 1,  0,  0, 0
	};

	glm::mat4x3 matThree = {
		0, 5, 2,
		0, 5, 12,
		10, 5, 12,
		10, 5, 2
	};

	glm::vec3 final = (matThree * matTwo) * matOne;


	//lights[0].position = final;

	r += 1;
	OM->GetObjects()[0]->Rotate(r, { 1, 0, 0 });

	if (!EventHandler::GetMouseEnabled())
		OM->GetObjects()[0]->Move(0.001f);
	
	OM->GetObjects()[1]->Move(_SHADOWSPEED);
	//std::cout << OM->GetObjects()[0]->GetPosition().x << std::endl;
	
	//camera->set_view_at(lights[0].position);
	glm::vec3 cameraPos = OM->GetObjects()[0]->GetPosition();
	cameraPos.z = camera->GetPosition().z;
	camera->set_view_from(cameraPos);

	
	//camera->set_view_from({ 30, 20, 50 });
	//camera->set_view_at({ 0, 0, 0 });

	glm::vec3 shadowPos = OM->GetObjects()[1]->GetPosition();
	shadowPos.z = _SHADOWCAMERAHEIGHT;
	renderer->m_shadowCamera->set_view_from(shadowPos);
	
	//renderer->m_shadowCamera->set_view_at({ 0, 0, 0 });
	//glm::vec3 shadowFor = shadowPos;
	//shadowFor.x -= 10;
	//renderer->m_shadowCamera->set_view_from(shadowPos);
	//renderer->m_shadowCamera->set_view_at(shadowFor);
	
	
	lights[0].position = shadowPos;
	//renderer->m_shadowCamera->set_view_from(lights[0].position);


	//camera->set_view_from({10, 10, 30};)

	//shadowPos.x = -shadowPos.x;
	//shadowPos.y = -shadowPos.y;
	//shadowPos.z = -shadowPos.z;

	//camera->set_view_from(shadowPos);
	//camera->set_view_at({ 0, 0, 3 });

	//camera->set_view_from(shadowPos);
	//camera->set_view_at(shadowFor);
}

void PlayGround::OnRender()
{
	renderer->Clear();
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilFunc(GL_ALWAYS, 0, GL_REPLACE);

	//renderer->DrawCube(*enviromentMap, {0, 0, 5 }, { 1, 1, 1 }, 90, 1, 0, 0);

	// glm::vec3 position, float rotation, float scale, PG2::Model model
	renderer->DrawModel(camera->GetPosition(), 0, 1, *skySphere);

	Texture* tex = new Texture("src/res/models/sphere/lebombo_prefiltered_env_map_001_2048.exr");

    renderer->DrawCube(*skyBox, camera->GetPosition(), { 1, 1, 1 }, 90, 1, 0, 0);
	//renderer->DrawMap(*map, { 0, 0, 0 }, lights, drawNormals);
	//renderer->DrawLight(lights[1]);
	
	
	//renderer->DrawModel({ 5, 8, -5 }, 0, 0.1, lights, *m);
	//renderer->DrawModel({ 0, 0, 0 }, 0, 0.1, lights, *m);
	renderer->DrawShadow({ 0, 0, 0 }, 0, 0.1, lights, *m);
	//renderer->DrawModel({ 5, 8, -5 }, 0, 1, lights, *m);
	//renderer->DrawModel(*diffuse, *specular, *normal, *ao, *roughness, { 5, 8, -5 }, 0, lights, ambientStrength, shininess, *model);

	//renderer->DrawLight(lights[0]);


	//renderer->DrawModel(*zidle_diffuse, *blank, *blank, *blank, *blank, { 0, 0, 0 }, 0, lights, ambientStrength, shininess, *model_zidle);
	//renderer->DrawModel(*stul_diffuse, *blank, *stul_normal, *blank, *blank, { 0, 0, 0 }, 0, lights, ambientStrength, shininess, *model_stul);
	//renderer->DrawModel(*kniha_diffuse, *blank, *kniha_normal, *blank, *blank, { 0, 0, 0 }, 0, lights, ambientStrength, shininess, *model_kniha);
	//renderer->DrawModel(*teren_diffuse, *blank, *blank, *blank, *blank, { 0, 0, 0 }, 0, lights, ambientStrength, shininess, *model_teren);
	//renderer->DrawModel(*vaza_diffuse, *blank, *blank, *blank, *blank, { 0, 0, 0 }, 0, lights, ambientStrength, shininess, *model_vaza);
	//renderer->DrawModel(*podlaha_diffuse, *blank, *blank, *blank, *blank, { 0, 0, 0 }, 0, lights, ambientStrength, shininess, *model_podlaha);
	//
	//renderer->DrawModel(*blank, *blank, *blank, *blank, *blank, { 5, 3, -5 }, 0, lights, ambientStrength, shininess, *model_sipka);
	
	

	renderer->DrawLine({ EventHandler::mouseRay->originPoint }, { EventHandler::mouseRay->destPoint });

	//renderer->DrawFont(*font, "NENI MUJ MODEL", glm::vec3(2, 11, -5), glm::vec3(0.0f));


	//for (Object* o : OM->GetObjects())
	//{
	//	glStencilFunc(GL_ALWAYS, o->GetID(), GL_REPLACE);
	//	o->Move(0.01f);
	//	renderer->DrawObject(*o);
	//}
	//
    //// rendering font
    //char test_char = 'C';
    //renderer->DrawChar(*font, test_char, glm::vec3(0, 5, 0), glm::vec3(1.0f));
	//
    //std::string test_text = "TOHLE JE TEST_123?!%.";
    //renderer->DrawFont(*font, test_text, glm::vec3(0, 8, 0), glm::vec3(0.0f));


}

void PlayGround::ImGuiOnUpdate()
{
	ImGui::Begin("Debug");
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
	//ImGui::SliderFloat("LightX", &lights[0].position.x, -20.0f, 20.0f);
	//ImGui::SliderFloat("LightY", &lights[0].position.y, -20.0f, 20.0f);
	//ImGui::SliderFloat("LightZ", &lights[0].position.z, 10.0f, 50.0f);
	ImGui::SliderFloat("shadowHeight", &_SHADOWCAMERAHEIGHT, 30.0f, 70.0f);
	ImGui::SliderFloat("shadowSpeed", &_SHADOWSPEED, 0.0005f, 0.01f);
	//ImGui::Separator();
	//ImGui::SliderFloat("ambientStrength", &ambientStrength, 0.0f, 1.0f);
	//ImGui::ColorEdit3("lightColor", glm::value_ptr(lights[0].color));
	//ImGui::SliderInt("shininess", &shininess, 32, 256);
	//ImGui::SliderFloat("SpecularStrength", &SpecularStrength, 0.0f, 1.0f);
	//ImGui::Separator();
	//ImGui::Checkbox("Show normals", &drawNormals);
	//if (ImGui::Button("Recalc map"))
	//{
	//	map->Recalc();
	//}
	ImGui::End();


}
