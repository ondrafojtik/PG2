#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 colorV;
layout(location = 3) in vec2 texCoord;
layout(location = 4) in vec3 tangent;
layout(location = 5) in vec3 bitangent;
layout(location = 6) in vec3 material_id;

struct Light
{
    vec3 position;
    vec3 color;
    vec3 info; // [type][cutoff][]
    vec3 lightDir;
};

struct Material
{
    float Ns;
    float Ni;
    vec3 Ka;
    vec3 Kd;
    vec3 Ks;
    vec3 Ke;
};

uniform Material materials[8];
out Material material;

uniform Light lights_out[2];
out Light lights[2];

out vec2 v_TexCoord;
out vec3 tangentViewPos;
out vec3 tangentFragPos;
out vec3 v_normal;
out vec3 v_FragPos;
out vec3 v_CameraPos;
out mat3 v_TBN;
out vec3 v_tang;

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;
uniform vec3 u_CameraPos;

void main()
{
    gl_Position = u_Projection * u_View * u_Model * vec4(position, 1.0);

    v_TexCoord = texCoord;

    //mat3 normalMatrix = transpose(inverse(mat3(u_Model)));
    //vec3 T = normalize(normalMatrix * tangent);
    //vec3 B = normalize(normalMatrix * bitangent);
    //vec3 N = normalize(normalMatrix * normal);
    ////vec3 B = normalize(cross(T, N));

    //mat3 normalMatrix = transpose(inverse(mat3(u_Model)));
    //vec3 T = normalize(normalMatrix * tangent);
    ////vec3 B = normalize(normalMatrix * bitangent);
    //vec3 N = normalize(normalMatrix * normal);
    //vec3 B = normalize(cross(T, N));

    //vec3 T = normalize(vec3(u_Model * vec4(tangent, 0.0)));
    //vec3 N = normalize(vec3(u_Model * vec4(normalize(normal), 0.0)));
    //T = normalize(T - dot(T, N) * N);
    //vec3 B = cross(N, T);
    //mat3 TBN = mat3(T, B, N);

    vec3 T = normalize(vec3(u_Model * vec4(tangent, 0.0)));
    vec3 B = normalize(vec3(u_Model * vec4(bitangent, 0.0)));
    vec3 N = normalize(vec3(u_Model * vec4(normal, 0.0)));
    mat3 TBN = mat3(T, B, N);

    vec3 v_FragPos = vec3(u_Model * vec4(position, 1.0));
    v_CameraPos = u_CameraPos;

    //mat3 TBN = transpose(mat3(T, B, N));
    v_TBN = transpose(TBN);
    tangentViewPos = TBN * u_CameraPos;
    tangentFragPos = TBN * v_FragPos;

    for (int i = 0; i < 2; i++)
    {
        lights[i].position = TBN * lights_out[i].position;
        //lights[i].position = lights_out[i].position;
        lights[i].color = lights_out[i].color;
        lights[i].info = lights_out[i].info;
        lights[i].lightDir = TBN * normalize(lights_out[i].lightDir);
        lights[i].lightDir = normalize(lights[i].lightDir);
    }

    
    v_normal = inverse(transpose(mat3(u_Model))) * normal;
    //v_normal = normal;

    int mat_index = int(material_id.r);
    v_tang = tangent;

    material = materials[mat_index];
    
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


struct Material
{
    float Ns;
    float Ni;
    vec3 Ka;
    vec3 Kd;
    vec3 Ks;
    vec3 Ke;
};

in Material material;


in Light lights[2];

in vec2 v_TexCoord;

in vec3 tangentViewPos;
in vec3 tangentFragPos;

in vec3 v_FragPos;
in vec3 v_normal;
in vec3 v_CameraPos;

in vec3 v_tang;

in mat3 v_TBN;

uniform sampler2D u_diffuseMap;
uniform sampler2D u_normalMap;
uniform sampler2D u_rmaMap;


//uniforms for testing 
uniform float u_AmbientStrength;
uniform float u_Shininess;

vec3 m_diffuse = texture(u_diffuseMap, v_TexCoord).rgb;
vec3 m_normal = texture(u_normalMap, v_TexCoord).rgb;
vec3 m_rma = texture(u_rmaMap, v_TexCoord).rgb;
float m_roughness = texture(u_rmaMap, v_TexCoord).r;
float m_metallnes = texture(u_rmaMap, v_TexCoord).g;
float m_AO = texture(u_rmaMap, v_TexCoord).b;


vec3 normal = normalize(v_TBN * normalize(m_normal * 2.0 - 1.0));  // this normal is in tangent space
//vec3 normal = normalize(m_normal * 2.0 - 1.0);  // this normal is in tangent space
//normal = normalize(v_TBN * normal);

//vec3 normal = normalize(m_normal);

vec3 calculate_point(Light l)
{
    vec3 albedo = m_diffuse * material.Kd;

    vec3 lightDir = normalize(l.position - tangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * albedo * l.color;
    //specular
    vec3 viewDir = normalize(tangentViewPos - tangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    //float random_metallness = mix(64, 32, (1.0 - m_specular)); 
    //float spec = pow(max(dot(normal, halfwayDir), 0.0), random_metallness);
    float shininess = 128;
    float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
    vec3 specular = m_roughness * spec * l.color;
    //vec3 specular = (1.0 - specStr) * spec * l.color;

    //result
    vec3 final = diffuse + specular;
    return final;
}

vec3 calculate_pointN(Light l)
{
    vec3 lightDir = normalize(l.position - v_FragPos);
    float diff = max(dot(lightDir, v_normal), 0.0);
    vec3 diffuse = diff * (m_diffuse * material.Kd) * l.color;
    //specular
    vec3 viewDir = normalize(v_CameraPos - v_FragPos);
    vec3 reflectDir = reflect(-lightDir, v_normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    //float random_metallness = mix(64, 32, (1.0 - m_specular)); 
    //float spec = pow(max(dot(normal, halfwayDir), 0.0), random_metallness);
    float shininess = 128;
    float spec = pow(max(dot(v_normal, halfwayDir), 0.0), shininess);
    vec3 specular = (1.0-m_roughness) * spec * l.color;
    //vec3 specular = (1.0 - specStr) * spec * l.color;

    //result
    vec3 final = diffuse + specular;
    return final;
}

vec3 NNcalculate_point(Light l)
{
    vec3 albedo = m_diffuse * material.Kd;
    vec3 lightDir = normalize(l.position - tangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * albedo * l.color;
    //specular
    vec3 viewDir = normalize(tangentViewPos - tangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    //float random_metallness = mix(64, 32, (1.0 - m_specular)); 
    //float spec = pow(max(dot(normal, halfwayDir), 0.0), random_metallness);
    //float spec = pow(max(dot(normal, halfwayDir), 0.0), u_Shininess);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32);
    vec3 specular = (1.0 - m_roughness) * spec * l.color;

    //result
    vec3 final = diffuse + specular;
    return final;
}

vec3 testlight(Light l)
{
    vec3 lightColor = l.color;
    vec3 ambient = material.Ka * lightColor;

    //diffuse
    vec3 norm = normalize(v_normal);
    vec3 lightDir = normalize(l.position - v_FragPos);
    float diffuseStregth = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diffuseStregth * lightColor * material.Kd;

    //specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(v_CameraPos - v_FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
    vec3 specular = specularStrength * spec * lightColor * material.Ks;

    //result
    vec3 result = (ambient + diffuse + specular);
    return result;
}

void main()
{
    vec3 diffuse_specular;
    
    //for (int i = 0; i < 1; i++)
    //{
    //    diffuse_specular += calculate_point(lights[i]);
    //}
    
    diffuse_specular += NNcalculate_point(lights[0]);

    // calculate ambient
    // vec3 ambient = m_diffuse * u_AmbientStrength * m_AO;
    vec3 albedo = m_diffuse * material.Kd;
    vec3 ambient = albedo * 0.5 * m_AO;

    vec3 finalColor = ambient + diffuse_specular;
    color = vec4(finalColor, 1.0);
    // calculate ambient
    //vec3 ambient = m_diffuse * material.Kd;
    //
    //vec3 finalColor = ambient + diffuse_specular;
    //color = vec4(finalColor, 1.0);
    
    //color = vec4(normal + 1.0 / 2.0, 1.0);

    
    //normal = normalize(v_TBN * normal);
    //color = vec4(normal + 1.0 / 2, 1.0);
    //color = vec4(v_tang, 1.0);

    //vec3 diffuse_specular;
    //diffuse_specular += testlight(lights[0]);
    //
    //color = vec4(diffuse_specular, 1.0);
}