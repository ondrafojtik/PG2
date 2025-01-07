#include "Ally.h"

Ally::Ally(glm::mat4x3 bezierPoints)
{

	matThree = bezierPoints;

	RenderData data;
	data.Init();

	vb = data.vb;
	layout = data.layout;
	va = new VertexArray();
	va->AddBuffer(*vb, layout);
	ib = data.ib;

}

Ally::~Ally()
{
	delete vb;
	delete va;
	delete ib;
}

void Ally::Move(float speed)
{

	timeStep += speed * direction;
	if (timeStep > 1)
		direction = -1;
	if (timeStep < 0)
		direction = 1;


	matOne = glm::vec4(timeStep * timeStep * timeStep, timeStep * timeStep, timeStep, 1);

	RecalcBezier();
}

void Ally::RecalcBezier()
{
	final = (matThree * matTwo) * matOne;
	Translate(final);
}
