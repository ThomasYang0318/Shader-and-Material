#version 330 core

in VS_OUT {
    vec3 Normal;
} fs_in;

out vec4 FragColor;

void main()
{
    // Visualize normal as RGB color
    // Normal values range from -1 to 1, convert to 0 to 1
    vec3 visualNormal = (fs_in.Normal + 1.0) * 0.5;
    FragColor = vec4(visualNormal, 1.0);
}
