#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;

uniform vec4 u_Color;

out vec4 v_color;
out vec3 v_normal;
out vec3 v_FragPos;

void main()
{
	gl_Position = (u_Projection * u_View * u_Model) * vec4(position, 1.0);
	v_color = vec4(0.05f, ((position.y + 1.0) / 2.0) * 0.5, 0.03f, 1.0f);
	//v_color = vec4(0.05f, (position.y + 1.0) / 2.0, 0.03f, 1.0f);
	//v_color = vec4(normal, 1.0);
	v_FragPos = vec3(u_Model * vec4(position, 1.0f));
	v_normal = inverse(transpose(mat3(u_Model))) * normal;
	

}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

struct Light
{
	vec3 position;
	vec3 color;
	vec3 info; // [type][cutoff][]
	vec3 lightDir;
};

uniform Light lights[2];

in vec4 v_color;
in vec3 v_normal;
in vec3 v_FragPos;

uniform float u_AmbientStrength;
uniform float u_Shininess;
uniform vec3 u_CameraPos;

vec3 calculate_light(Light light)
{
	//diffuse
	vec3 norm = normalize(v_normal);
	vec3 lightDir = normalize(light.position - v_FragPos);
	float diffuseStregth = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diffuseStregth * light.color * vec3(v_color.x, v_color.y, v_color.z);

	//specular
	float specularStrength = 0.5;
	vec3 viewDir = normalize(u_CameraPos - v_FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Shininess);
	vec3 specular = specularStrength * spec * light.color;

	//result
	return vec3(diffuse + specular);
}

void main()
{
	vec3 result = vec3(0, 0, 0);

	for(int i = 0; i < 2; i++)
	{
		result += calculate_light(lights[i]);
	}

	//ambient
	
	vec4 ambient = v_color * u_AmbientStrength;
	
	color = ambient + vec4(result, 1.0);
}