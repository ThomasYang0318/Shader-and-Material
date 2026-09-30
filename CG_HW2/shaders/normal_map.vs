#version 330 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 TexCoord;
layout (location = 3) in vec3 Tangent;

uniform mat4 MVP;
uniform mat4 Model;
uniform mat3 NormalMatrix;

out VS_OUT {
    vec3 FragPos;
    vec2 TexCoord;
    mat3 TBN;
} vs_out;

void main()
{
    vs_out.FragPos = vec3(Model * vec4(Position, 1.0));
    vs_out.TexCoord = TexCoord;
    
    vec3 T = normalize(vec3(Model * vec4(Tangent, 0.0)));
    vec3 N = normalize(NormalMatrix * Normal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    
    vs_out.TBN = mat3(T, B, N);
    gl_Position = MVP * vec4(Position, 1.0);
}
