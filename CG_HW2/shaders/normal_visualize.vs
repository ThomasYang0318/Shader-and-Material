#version 330 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;

uniform mat4 MVP;
uniform mat3 NormalMatrix;

out VS_OUT {
    vec3 Normal;
} vs_out;

void main()
{
    vs_out.Normal = normalize(NormalMatrix * Normal);
    gl_Position = MVP * vec4(Position, 1.0);
}
