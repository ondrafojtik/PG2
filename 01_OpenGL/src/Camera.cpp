#include "Camera.h"
#define M_PI_2     1.57079632679489661923   // pi/2

Camera::Camera(const glm::vec3& position, float fov, AspectRatio ar, float zNear, float zFar)
	: m_Position(position), m_near(zNear), m_far(zFar),
	forward(glm::vec3(0.0f, 0.0f, -1.0f)), up(glm::vec3(0.0f, 1.0f, 0.0f)), rotationProduct(glm::cross(forward, up))
{
	perspective = glm::perspective(fov, (float)ar.x / ar.y, zNear, zFar);
	RecalcView();
	oldMousePos = glm::dvec2(1920, 1080);
}


void Camera::mouseUpdate(const glm::dvec2& newMousePosition)
{
	glm::dvec2 delta = (oldMousePos - newMousePosition);
	
	float mouseSesitivity = 0.5f;
	rotationProduct = glm::cross(forward, up); //to rotate around!
	//creating rot mat
	glm::mat4 rotationMat = glm::rotate(glm::mat4(1.0f), glm::radians((float)delta.x * mouseSesitivity), up) 
		* glm::rotate(glm::mat4(1.0f), glm::radians((float)delta.y * mouseSesitivity), rotationProduct);
	forward = glm::mat3(rotationMat) * forward; //final product
	RecalcView();
	//UpdateView();

	oldMousePos = newMousePosition;
	NotifyObservers();
}


// PG2

/*
void Camera::mouseUpdate(const glm::dvec2& newMousePosition)
{
	glm::dvec2 delta = (oldMousePos - newMousePosition);

	float mouseSesitivity = 0.5f;
	rotationProduct = glm::cross(forward, up); //to rotate around!
	//creating rot mat
	glm::mat4 rotationMat = glm::rotate(glm::mat4(1.0f), glm::radians((float)delta.x * mouseSesitivity), up)
		* glm::rotate(glm::mat4(1.0f), glm::radians((float)delta.y * mouseSesitivity), rotationProduct);
	forward = glm::mat3(rotationMat) * forward; //final product
	UpdateView();

	oldMousePos = newMousePosition;
	NotifyObservers();
}
*/

Camera::Camera(const int width, const int height, const float fov_y, const glm::vec3 view_from, const glm::vec3 view_at)
{
	m_Position = view_from;
	up = glm::vec3(0.0f, 0.0f, 1.0f);
	forward = glm::vec3(1.0f, 0.0f, 0.0f);
	rotationProduct = glm::cross(forward, up); //to rotate around!

	width_ = width;
	height_ = height;
	fov_y_ = fov_y;

	view_from_ = view_from;
	view_at_ = view_at;


	Update();
}

glm::vec3 Camera::view_from() const
{
	return view_from_;
}

glm::mat3 Camera::M_c_w() const
{
	return M_c_w_;
}

float Camera::focal_length() const
{
	return f_y_;
}

void Camera::set_fov_y(const float fov_y)
{
	assert(fov_y > 0.0);

	fov_y_ = fov_y;
}

void Camera::Update()
{
	m_Position = view_from_;

	UpdateView();
	UpdateProjection();
	
	float t = 0.1;
}

void Camera::UpdateView()
{
	f_y_ = height_ / (2.0f * tanf(fov_y_ * 0.5f));

	view_from_ = m_Position;
	// view matrix

	glm::vec3 z_c = view_from_ - view_at_;

	z_c = glm::normalize(z_c);
	glm::vec3 x_c = glm::cross(up_, z_c);
	x_c = glm::normalize(x_c);
	glm::vec3 y_c = glm::cross(z_c, x_c);
	y_c = glm::normalize(y_c);
	
	{
		//glm::vec4 r0 = glm::vec4(x_c.x, x_c.y, x_c.z, 0.0f);
		//glm::vec4 r1 = glm::vec4(y_c.x, y_c.y, y_c.z, 0.0f);
		//glm::vec4 r2 = glm::vec4(z_c.x, z_c.y, z_c.z, 0.0f);
		//glm::vec4 r3 = glm::vec4(view_from_.x, view_from_.y, view_from_.z, 1.0f);
		//ViewMatrix = glm::mat4(r0, r1, r2, r3);
	}
	
	glm::mat4 v;
	{
		//float data[16] = {
		//	x_c.x, y_c.x, z_c.x, view_from_.x,
		//	x_c.y, y_c.y, z_c.y, view_from_.y,
		//	x_c.z, y_c.z, z_c.z, view_from_.z,
		//	0.0f, 0.0f, 0.0f, 1.0f
		//};

		float data[16] = {
			x_c.x, x_c.y, x_c.z, 0.0f,
			y_c.x, y_c.y, y_c.z, 0.0f,
			z_c.x, z_c.y, z_c.z, 0.0f,
			view_from_.x, view_from_.y, view_from_.z, 1.0f
		};

		v = glm::make_mat4(data);
	}
	ViewMatrix = v;

	//glm::vec4 r0 = glm::vec4(x_c.x, y_c.x, z_c.x, view_from_.x);
	//glm::vec4 r1 = glm::vec4(x_c.y, y_c.y, z_c.y, view_from_.y);
	//glm::vec4 r2 = glm::vec4(x_c.z, y_c.z, z_c.z, view_from_.z);
	//glm::vec4 r3 = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);


	

	ViewMatrix = glm::inverse(ViewMatrix);

	//NotifyObservers();
}

glm::mat4 Camera::get_view()
{
	//glm::vec3 from = {30, 20, 50};
	//glm::vec3 to = {0, 0, 0};

	UpdateView();
	return ViewMatrix;

	//f_y_ = height_ / (2.0f * tanf(fov_y_ * 0.5f));
	//
	//view_from_ = m_Position;
	//// view matrix
	//
	//glm::vec3 from = view_from_;
	//glm::vec3 to = view_at_;
	//
	//glm::vec3 z_c = from - to;
	//
	//z_c = glm::normalize(z_c);
	//glm::vec3 x_c = glm::cross(up_, z_c);
	//x_c = glm::normalize(x_c);
	//glm::vec3 y_c = glm::cross(z_c, x_c);
	//y_c = glm::normalize(y_c);
	//
	//glm::vec4 r0 = glm::vec4(x_c.x, x_c.y, x_c.z, 0.0f);
	//glm::vec4 r1 = glm::vec4(y_c.x, y_c.y, y_c.z, 0.0f);
	//glm::vec4 r2 = glm::vec4(z_c.x, z_c.y, z_c.z, 0.0f);
	//glm::vec4 r3 = glm::vec4(view_from_.x, view_from_.y, view_from_.z, 1.0f);
	//
	////glm::vec4 r0 = glm::vec4(x_c.x, y_c.x, z_c.x, from.x);
	////glm::vec4 r1 = glm::vec4(x_c.y, y_c.y, z_c.y, from.y);
	////glm::vec4 r2 = glm::vec4(x_c.z, y_c.z, z_c.z, from.z);
	////glm::vec4 r3 = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	//
	//
	//glm::mat4 __view = glm::mat4(r0, r1, r2, r3);
	//
	//__view = glm::inverse(ViewMatrix);
	//
	//return __view;
}

glm::mat4 Camera::get_projection()
{
	int n = nearProjection;
	int f = farProjection;
	int a = (n + f) / (n - f);
	int b = (2 * n * f) / (n - f);

	glm::mat4 D;
	{
		float data[16] = {
			n, 0, 0, 0,
			0, n, 0, 0,
			0, 0, a, -1,
			0, 0, b, 0
		};


		D = glm::make_mat4(data);
	}

	{
		//glm::vec4 r0 = glm::vec4(n, 0, 0, 0);
		//glm::vec4 r1 = glm::vec4(0, n, 0, 0);
		//glm::vec4 r2 = glm::vec4(0, 0, a, b);
		//glm::vec4 r3 = glm::vec4(0, 0, -1, 0);
		//D = glm::mat4(r0, r1, r2, r3);
	}

	float aspect = (float)width_ / (float)height_;
	//float w = 2.0f * n * tan(fov_y_ / 2.0f);
	//float h = w * (1.0f / aspect);

	float h = 1.0f / tan(fov_y_ / 2);
	float w = 1.0f / (aspect * tan(fov_y_ / 2));

	glm::mat4 N;
	{
		float data[16] = {
			w, 0,   0, 0,
			0,        h, 0, 0,
			0,        0,   1, 0,
			0,        0,   0, 1
		};


		N = glm::make_mat4(data);
	}

	{
		//glm::vec4 r0 = glm::vec4(2.0f / w, 0, 0, 0);
		//glm::vec4 r1 = glm::vec4(0, 2 / h, 0, 0);
		//glm::vec4 r2 = glm::vec4(0, 0, 1, 0);
		//glm::vec4 r3 = glm::vec4(0, 0, 0, 1);
		//N = glm::mat4(r0, r1, r2, r3);
	}

	//ProjectionMatrix = glm::matrixCompMult(N, D);
	glm::mat4 __proj = N * D;

	return __proj;
}


void Camera::UpdateProjection()
{
	// projetion

	int n = nearProjection;
	int f = farProjection;
	int a = (n + f) / (n - f);
	int b = 2 * n * f / (n - f);

	glm::mat4 D;
	{
		float data[16] = {
			n, 0, 0, 0,
			0, n, 0, 0,
			0, 0, a, b,
			0, 0, -1, 0
		};


		D = glm::make_mat4(data);
	}

	{
		glm::vec4 r0 = glm::vec4(n, 0, 0, 0);
		glm::vec4 r1 = glm::vec4(0, n, 0, 0);
		glm::vec4 r2 = glm::vec4(0, 0, a, b);
		glm::vec4 r3 = glm::vec4(0, 0, -1, 0);
		D = glm::mat4(r0, r1, r2, r3);
	}

	float aspect = (float)width_ / (float)height_;
	float w = 2 * n * tan(fov_y_ / 2);
	float h = w * (1 / aspect);

	glm::mat4 N;
	{
		float data[16] = {
			2.0f / w, 0,   0, 0,
			0,        2 / h, 0, 0,
			0,        0,   1, 0,
			0,        0,   0, 1
		};


		N = glm::make_mat4(data);
	}

	{
		glm::vec4 r0 = glm::vec4(2.0f / w, 0, 0, 0);
		glm::vec4 r1 = glm::vec4(0, 2 / h, 0, 0);
		glm::vec4 r2 = glm::vec4(0, 0, 1, 0);
		glm::vec4 r3 = glm::vec4(0, 0, 0, 1);
		N = glm::mat4(r0, r1, r2, r3);
	}

	//ProjectionMatrix = glm::matrixCompMult(N, D);
	ProjectionMatrix = N * D;

}
