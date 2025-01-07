#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 colorV;
layout(location = 3) in vec2 texCoord;
layout(location = 4) in vec3 tangent;
layout(location = 5) in vec3 bitangent;
layout(location = 6) in vec3 material_id;

uniform mat4 u_lightProjection;
uniform mat4 u_lightView;
uniform mat4 u_Model;
uniform mat4 MLP;

void main()
{
	//gl_Position = (u_lightProjection * u_lightView) * u_Model * vec4(position, 1.0);
	gl_Position = MLP * vec4(position, 1.0);
}

#shader fragment
#version 330 core

//layout(location = 0) out vec4 color;


void main()
{
	gl_FragDepth = gl_FragCoord.z;
	//gl_FragDepth = 0.0;
}