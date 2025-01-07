#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;

struct Light
{
    vec3 position;
    vec3 color;
    vec3 info; // [type][cutoff][]
    vec3 lightDir;
};

uniform Light lights_out[2];
out Light lights[2];

out vec2 v_TexCoord;
out vec3 tangentViewPos;
out vec3 tangentFragPos;

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;
uniform vec3 u_CameraPos;

void main()
{
    gl_Position = u_Projection * u_View * u_Model * vec4(position, 1.0);

    v_TexCoord = texCoord;

    mat3 normalMatrix = transpose(inverse(mat3(u_Model)));
    vec3 T = normalize(normalMatrix * tangent);
    //vec3 B = normalize(normalMatrix * bitangent);
    vec3 N = normalize(normalMatrix * normal);
    vec3 B = normalize(cross(T, N));

    vec3 v_FragPos = vec3(u_Model * vec4(position, 1.0));

    mat3 TBN = transpose(mat3(T, B, N));
    tangentViewPos = TBN * u_CameraPos;
    tangentFragPos = TBN * v_FragPos;

    for (int i = 0; i < 2; i++)
    {
        lights[i].position = TBN * lights_out[i].position;
        lights[i].color = lights_out[i].color;
        lights[i].info = lights_out[i].info;
        lights[i].lightDir = TBN * normalize(lights_out[i].lightDir);
    }


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

in Light lights[2];

in vec2 v_TexCoord;

in vec3 tangentViewPos;
in vec3 tangentFragPos;

uniform sampler2D u_diffuseMap;
uniform sampler2D u_specularMap;
uniform sampler2D u_AO;
uniform sampler2D u_roughness;
uniform sampler2D u_normalMap;

//uniforms for testing 
uniform float u_AmbientStrength;
uniform float u_Shininess;

vec3 m_diffuse = texture(u_diffuseMap, v_TexCoord).rgb;
float m_specular = texture(u_specularMap, v_TexCoord).r;
vec3 m_normal = texture(u_normalMap, v_TexCoord).rgb;
float AO = texture(u_AO, v_TexCoord).r;
float specularStrength = texture(u_roughness, v_TexCoord).r;

vec3 normal = normalize(m_normal * 2.0 - 1.0);  // this normal is in tangent space

vec3 calculate_point(Light l)
{
    vec3 lightDir = normalize(l.position - tangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * m_diffuse * l.color;
    //specular
    vec3 viewDir = normalize(tangentViewPos - tangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    //float random_metallness = mix(64, 32, (1.0 - m_specular)); 
    //float spec = pow(max(dot(normal, halfwayDir), 0.0), random_metallness);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), u_Shininess);
    vec3 specular = (1.0 - specularStrength) * spec * l.color;
    
    //result
    vec3 final = diffuse + specular;
    return final;
}

vec3 calculate_directional(Light l)
{
    vec3 lightDir = normalize(-l.lightDir);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * m_diffuse * l.color;
    //specular
    vec3 viewDir = normalize(tangentViewPos - tangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    //float random_metallness = mix(64, 32, (1.0 - m_specular)); 
    //float spec = pow(max(dot(normal, halfwayDir), 0.0), random_metallness);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), u_Shininess);
    vec3 specular = (1.0 - specularStrength) * spec * l.color;
    
    //result
    vec3 final = diffuse + specular;
    return final;
}

vec3 calculate_spotlight(Light l)
{
    vec3 lightDir = normalize(l.position - tangentFragPos);
    float theta = dot(lightDir, normalize(-l.lightDir));

    vec3 final = vec3(0, 0, 0);

    if (theta > l.info.g)
    {
        vec3 lightDir = normalize(-l.lightDir);
        float diff = max(dot(lightDir, normal), 0.0);
        vec3 diffuse = diff * m_diffuse * l.color;
        //specular
        vec3 viewDir = normalize(tangentViewPos - tangentFragPos);
        vec3 reflectDir = reflect(-lightDir, normal);
        vec3 halfwayDir = normalize(lightDir + viewDir);
        //float random_metallness = mix(64, 32, (1.0 - m_specular)); 
        //float spec = pow(max(dot(normal, halfwayDir), 0.0), random_metallness);
        float spec = pow(max(dot(normal, halfwayDir), 0.0), u_Shininess);
        vec3 specular = (1.0 - specularStrength) * spec * l.color;
    
        //result
        vec3 final = diffuse + specular;
        return final;
    }

    return final;
}

void main()
{
    vec3 diffuse_specular;

    for (int i = 0; i < 2; i++)
    {
        // switch-like branching

        if (lights[i].info.x == 0)                  // point 
        {
            diffuse_specular += calculate_point(lights[i]);
        }
        if (lights[i].info.x == 1)             // directional 
        {
            diffuse_specular += calculate_directional(lights[i]);
        }
        if (lights[i].info.x == 2)              // spotlight
        {
            diffuse_specular += calculate_spotlight(lights[i]);
        }
        
        
    }
    
    // calculate ambient
    vec3 ambient = m_diffuse * u_AmbientStrength * AO;

    vec3 finalColor = ambient + diffuse_specular;
    color = vec4(finalColor, 1.0);

    color = vec4(normal, 1.0f);
}