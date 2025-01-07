#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 colorV;
layout(location = 3) in vec2 texCoord;
layout(location = 4) in vec3 tangent;
layout(location = 5) in vec3 bitangent;
layout(location = 6) in vec3 material_id;

struct Material
{
    float Ns;
    float Ni;
    vec3 Ka;
    vec3 Kd;
    vec3 Ks;
    vec3 Ke;
};

out vec2 v_TexCoord;
out vec3 v_position;
out Material material;
out vec3 v_normal;

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;
uniform Material materials[8];

void main()
{
	gl_Position = (u_Projection * u_View * u_Model) * vec4(position, 1.0);
	v_TexCoord = texCoord;

    int mat_index = int(material_id.r);
    material = materials[mat_index];

    v_normal = normal;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

struct Material
{
    float Ns;
    float Ni;
    vec3 Ka;
    vec3 Kd;
    vec3 Ks;
    vec3 Ke;
};

in vec2 v_TexCoord;
in vec3 v_position;
in Material v_material;
in vec3 v_normal;

uniform sampler2D u_diffuseMap;

const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

void main()
{
    vec2 res = SampleSphericalMap(v_normal);
    
    vec3 diffuseMap = texture(u_diffuseMap, res).rgb;

	color = vec4(diffuseMap, 1.0);
    
    //color = vec4(res.x, res.y, 0.0f, 1.0f);
    
    //color = vec4(diffuseMap * v_material.Kd, 1.0f);

}