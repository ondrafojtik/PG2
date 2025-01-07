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

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;
uniform vec3 u_CameraPos;

uniform mat4 u_lightView;
uniform mat4 u_lightProjection;

uniform mat4 MVP;
uniform mat4 MDL;
uniform mat4 MLP;
// MVP
// MDL
// MLP


out vec3 position_lcs;

void main()
{
    gl_Position = u_Projection * u_View * u_Model * vec4(position, 1.0);

    gl_Position = MVP * vec4(position, 1.0);

    //mat4 mlp = u_lightProjection * u_lightView * u_Model;// * vec4(position, 1.0);
    //vec4 tmp = mlp * vec4(position.xyz, 1.0);

    vec4 tmp = (u_lightProjection * u_lightView * u_Model) * vec4(position.xyz, 1.0);
    tmp = MLP * vec4(position, 1.0);

    position_lcs = tmp.xyz / tmp.w;


    v_TexCoord = texCoord;

    mat3 normalMatrix = transpose(inverse(mat3(u_Model)));
    vec3 T = normalize(normalMatrix * tangent);
    vec3 B = normalize(normalMatrix * bitangent);
    vec3 N = normalize(normalMatrix * normal);
    //vec3 B = normalize(cross(T, N));

    v_FragPos = vec3(u_Model * vec4(position, 1.0));
    v_FragPos = vec3(MDL * vec4(position, 1.0));
    //v_FragPos = vec3(u_Projection * u_View * u_Model * vec4(position, 1.0));
    v_CameraPos = u_CameraPos;

    mat3 TBN = mat3(T, B, N);
    tangentViewPos = TBN * u_CameraPos;
    tangentFragPos = TBN * v_FragPos;

    for (int i = 0; i < 2; i++)
    {
        //lights[i].position = TBN * lights_out[i].position;
        lights[i].position = lights_out[i].position;
        lights[i].color = lights_out[i].color;
        lights[i].info = lights_out[i].info;
        lights[i].lightDir = TBN * normalize(lights_out[i].lightDir);
    }

    
    //v_normal = inverse(transpose(mat3(u_Model))) * normal;
    v_normal = normal;

    int mat_index = int(material_id.r);

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

in vec3 position_lcs;

uniform sampler2D u_diffuseMap;
uniform sampler2D u_specularMap;
uniform sampler2D u_AO;
uniform sampler2D u_roughness;
uniform sampler2D u_normalMap;
uniform sampler2D u_shadowMap;

//uniforms for testing 
uniform float u_AmbientStrength;
uniform float u_Shininess;

vec3 m_shadow = texture(u_shadowMap, v_TexCoord).rgb;
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

vec3 calculate_point_new(Light l)
{
    vec3 lightDir = normalize(l.position - tangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    //m_diffuse = material.Kd;
    vec3 diffuse = diff * m_diffuse * l.color;
    //specular
    vec3 viewDir = normalize(tangentViewPos - tangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    //float random_metallness = mix(64, 32, (1.0 - m_specular)); 
    //float spec = pow(max(dot(normal, halfwayDir), 0.0), random_metallness);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), u_Shininess);
    vec3 specular = (1.0 - material.Ns) * spec * l.color;

    //result
    vec3 final = diffuse + specular;
    return final;
}

vec3 calculate_point_new2(Light l)
{
    // ambient
    vec3 light_color = l.color;
    vec3 ambientStrength = material.Ka;
    vec3 ambient = ambientStrength * light_color;

    // diffuse
    vec3 norm = normalize(v_normal);
    vec3 lightDir = normalize(l.position - v_FragPos);
    float diffuseStrength = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = diffuseStrength * l.color * material.Kd;

    // spec
    float specularStrength = material.Ns;
    vec3 viewDir = normalize(v_CameraPos - v_FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.Ns);
    vec3 specular = spec * l.color * material.Ks;
    
    return (ambient + diffuse + specular);
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

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

void main()
{
    //vec3 diffuse_specular;
    //
    //for (int i = 0; i < 2; i++)
    //{
    //    // switch-like branching
    //
    //    if (lights[i].info.x == 0)                  // point 
    //    {
    //        diffuse_specular += calculate_point_new(lights[i]);
    //    }
    //    if (lights[i].info.x == 1)             // directional 
    //    {
    //        //diffuse_specular += calculate_directional(lights[i]);
    //    }
    //    if (lights[i].info.x == 2)              // spotlight
    //    {
    //        //diffuse_specular += calculate_spotlight(lights[i]);
    //    }
    //    
    //    
    //}
    //
    //// calculate ambient
    ////m_diffuse = material.Kd;
    //vec3 ambient_color = material.Ka;
    ////vec3 ambient = m_diffuse * u_AmbientStrength * AO;
    //vec3 ambient = m_diffuse * u_AmbientStrength * ambient_color;
    //
    //vec3 finalColor = ambient + diffuse_specular;
    //color = vec4(finalColor, 1.0);
    //
    //vec3 result = vec3(0.0f, 0.0f, 0.0f);
    //vec3 albedo = material.Kd;
    //vec3 metallic = material.Ks;
    //
    //vec3 N = normalize(v_normal); // jeste asi nemusim delat to divny co delam ve vertexu
    //vec3 V = normalize(v_CameraPos - v_FragPos);
    //
    //for (int i = 0; i < 2; i++)
    //{
    //    // all lights
    //    vec3 L = normalize(lights[i].position - v_FragPos);
    //    vec3 H = normalize(V + L);
    //
    //    float distance = length(lights[i].position - v_FragPos);
    //    float attenuation = 1.0 / (distance * distance);
    //    vec3 radiance = lights[i].color * attenuation;
    //
    //    vec3 F0 = vec3(0.04);
    //    F0 = mix(F0, albedo, metallic); // todo metallic
    //    vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);
    //
    //    result += F;
    //}



    //vec3 albedo = m_diffuse * material.Kd;
    //color = vec4(albedo, 1.0);
    //vec3 norm = normalize(v_normal);
    //vec3 V = normalize(v_CameraPos - v_FragPos);
    //float cosTheta = max(dot(norm, V), 0.0);

    //color = vec4(albedo * cosTheta, 1.0f);

    //color = vec4(result, 1.0);

    //color = vec4(m_diffuse, 1.0f);
    //color = vec4(1.0f, 0.0f, 0.0f, 1.0f);


    //color = vec4(material.Kd * m_diffuse, 1.0f);

    //vec2 a_tc = (position_lcs.xy + vec2(1.0f)) * 0.5f;
    ////vec2 a_tc = (position_lcs.xy * 0.5f + 0.5);
    //float depth = texture(u_shadowMap, a_tc).r;
    //float bias = 0.001;
    //depth = depth * 2.0f - 1.0f;
    //float shadow = (depth + bias >= position_lcs.z) ? 1.0f : 0.25f;


    // PCF
    vec2 shadow_texel_size = 1.0f / textureSize(u_shadowMap, 0);
    const int r = 2;
    float shadow = 0.0f;
    for (int y = -r; y <= r; y++)
        for (int x = -r; x <= r; x++)
        {
            vec2 a_tc = (position_lcs.xy + vec2(1.0f)) * 0.5f;
            a_tc += vec2(x, y) * shadow_texel_size;
            float depth = texture(u_shadowMap, a_tc).r;
            depth = depth * 2.0f - 1.0f;
            float bias = 0.001f;
            shadow += (depth + bias >= position_lcs.z) ? 1.0f : 0.25f;
        }
    shadow *= (1.0f / ((2 * r + 1) * (2 * r + 1)));


    vec3 albedo = m_diffuse * material.Kd;
    //vec3 albedo = m_shadow * material.Kd;
    vec3 ambient = material.Ka * lights[0].color;
    
    vec3 norm = normalize(v_normal);

    //vec3 norm = texture(u_normalMap, v_TexCoord).rgb;
    //norm = norm * 2.0 - 1.0;
    //norm = normalize(v_TBN * norm);
    
    vec3 viewDir = normalize(v_CameraPos - v_FragPos); 
    float flipped = dot(norm, viewDir);
    if (flipped < 0)
        norm = -norm;
    
    
    vec3 lightPos = lights[0].position;
    vec3 lightDir = normalize(lightPos - v_FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lights[0].color * (diff * material.Kd);
    //tangentViewPos = v_TBN * u_CameraPos;
    //tangentFragPos = v_TBN * v_FragPos;
    
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.Ns);
    vec3 specular = lights[0].color * (spec * material.Ks);
    
    vec3 result = (ambient + diffuse + specular) * albedo * shadow;
    
    color = vec4(result, 1.0);

    //color = vec4(albedo, 1.0);

    //color = vec4(norm * 0.5 + 0.5, 1.0f);


}