#version 330 core

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoord;
    mat3 TBN;
} fs_in;

// Material Properties
uniform vec3 Ka;      // Ambient coefficient
uniform vec3 Kd;      // Diffuse coefficient
uniform vec3 Ks;      // Specular coefficient
uniform float Ns;     // Shininess (specular exponent)

// Textures
uniform sampler2D DiffuseMap;
uniform sampler2D NormalMap;

// Lighting
uniform vec3 LightPos;
uniform vec3 ViewPos;
uniform vec3 LightColor;

out vec4 FragColor;

void main()
{
    // Sample normal from normal map and transform to world space
    vec3 norm = texture(NormalMap, fs_in.TexCoord).rgb;
    norm = normalize(norm * 2.0 - 1.0);
    norm = normalize(fs_in.TBN * norm);
    
    // Sample diffuse color from texture
    vec3 texColor = texture(DiffuseMap, fs_in.TexCoord).rgb;
    
    vec3 lightDir = normalize(LightPos - fs_in.FragPos);
    vec3 viewDir = normalize(ViewPos - fs_in.FragPos);
    
    // Ambient
    vec3 ambient = Ka * LightColor;
    
    // Diffuse
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * Kd * texColor * LightColor;
    
    // Specular
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), Ns);
    vec3 specular = spec * Ks * LightColor;
    
    // Combine
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
