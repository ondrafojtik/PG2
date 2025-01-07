#shader vertex
#version 330 core

layout(location = 0) in vec3 position;

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;

uniform vec4 u_Color;

out vec4 v_color;

void main()
{
	gl_Position = (u_Projection * u_View * u_Model) * vec4(position, 1.0);
	v_color = u_Color;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec4 v_color;

void main()
{
	color = v_color;
}