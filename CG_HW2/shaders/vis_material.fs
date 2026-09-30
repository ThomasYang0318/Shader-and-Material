#version 330 core

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
} fs_in;

// Material Properties
uniform vec3 Ka;      // Ambient coefficient
uniform vec3 Kd;      // Diffuse coefficient
uniform vec3 Ks;      // Specular coefficient
uniform float Ns;     // Shininess (specular exponent)

// Lighting
uniform vec3 LightPos;
uniform vec3 ViewPos;
uniform vec3 LightColor;

out vec4 FragColor;

void main()
{
    // Normalize vectors
    vec3 norm = normalize(fs_in.Normal);
    vec3 lightDir = normalize(LightPos - fs_in.FragPos);
    vec3 viewDir = normalize(ViewPos - fs_in.FragPos);
    
    // Ambient
    vec3 ambient = Ka * LightColor;
    
    // Diffuse
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * Kd * LightColor;
    
    // Specular
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), Ns);
    vec3 specular = spec * Ks * LightColor;
    
    // Combine
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
