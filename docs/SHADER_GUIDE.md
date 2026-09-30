# Shader Guide - Detailed Reference

This guide provides comprehensive documentation for all shaders in the CG_HW2 project.

## Table of Contents

1. [Shader Overview](#shader-overview)
2. [Phong Lighting Shader](#phong-lighting-shader)
3. [Normal Mapping Shader](#normal-mapping-shader)
4. [Unlit Shader](#unlit-shader)
5. [Normal Visualization Shader](#normal-visualization-shader)
6. [Uniform Variables](#uniform-variables)
7. [Vertex Attributes](#vertex-attributes)
8. [GLSL Basics](#glsl-basics)

---

## Shader Overview

All shaders follow the OpenGL 3.3 core profile specification. Each shader pair consists of:

- **Vertex Shader (.vs)**: Processes per-vertex data, transforms positions, calculates per-vertex lighting
- **Fragment Shader (.fs)**: Determines final pixel color, handles texture sampling, per-fragment lighting

### Shader Loading

Shaders are loaded and compiled at runtime:

```cpp
ShaderProg shader;
bool loaded = shader.LoadFromFiles("shaders/vis_material.vs", "shaders/vis_material.fs");
```

---

## Phong Lighting Shader

**Files**: `vis_material.vs`, `vis_material.fs`

### Purpose
Implements the Phong illumination model with ambient, diffuse, and specular components for realistic lighting.

### Vertex Shader (vis_material.vs)

```glsl
#version 330 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;

uniform mat4 MVP;
uniform mat4 Model;
uniform mat3 NormalMatrix;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
} vs_out;

void main()
{
    vs_out.FragPos = vec3(Model * vec4(Position, 1.0));
    vs_out.Normal = normalize(NormalMatrix * Normal);
    gl_Position = MVP * vec4(Position, 1.0);
}
```

**What it does:**
1. Transforms vertex position to world space using `Model` matrix
2. Transforms normal to world space using `NormalMatrix` (inverse transpose of Model)
3. Outputs final clip-space position using MVP matrix
4. Passes world-space position and normal to fragment shader

**Key Points:**
- `NormalMatrix = transpose(inverse(Model))` - must be calculated in C++
- Normals must be normalized for correct lighting calculations
- Position in world space is needed for lighting calculations in fragment shader

### Fragment Shader (vis_material.fs)

```glsl
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
```

**Phong Model Breakdown:**

1. **Ambient Component** (`I_a = Ka * La`)
   - Represents ambient light in the scene
   - Does NOT depend on light direction
   - Typically low intensity to prevent completely dark shadows

2. **Diffuse Component** (`I_d = Kd * (N · L) * Ld`)
   - Represents light scattered equally in all directions
   - Depends on angle between normal (N) and light direction (L)
   - `max(dot(norm, lightDir), 0.0)` prevents backlit surfaces

3. **Specular Component** (`I_s = Ks * (V · R)^Ns * Ls`)
   - Represents bright highlights
   - Depends on view direction (V) and reflection direction (R)
   - `Ns` controls highlight sharpness (higher = sharper highlight)

**Setting Up C++:**

```cpp
// Create shader
VisMaterialShaderProg shader;
shader.LoadFromFiles("shaders/vis_material.vs", "shaders/vis_material.fs");

// Set up matrices
glm::mat4 mvp = projection * view * model;
glm::mat4 normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));

// Set uniforms (pseudo-code)
glUniformMatrix4fv(shader.GetLocMVP(), 1, GL_FALSE, &mvp[0][0]);
glUniformMatrix4fv(locModel, 1, GL_FALSE, &model[0][0]);
glUniformMatrix3fv(locNormalMatrix, 1, GL_FALSE, &normalMatrix[0][0]);

// Material
glUniform3f(locKa, material.Ka.x, material.Ka.y, material.Ka.z);
glUniform3f(locKd, material.Kd.x, material.Kd.y, material.Kd.z);
glUniform3f(locKs, material.Ks.x, material.Ks.y, material.Ks.z);
glUniform1f(locNs, material.Ns);

// Lighting
glUniform3f(locLightPos, light.x, light.y, light.z);
glUniform3f(locViewPos, camera.x, camera.y, camera.z);
glUniform3f(locLightColor, 1.0f, 1.0f, 1.0f);
```

---

## Normal Mapping Shader

**Files**: `normal_map.vs`, `normal_map.fs`

### Purpose
Implements Phong lighting with normal mapping to add surface detail without additional geometry.

### Vertex Shader (normal_map.vs)

```glsl
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
    
    // Gram-Schmidt orthogonalization for TBN
    vec3 T = normalize(vec3(Model * vec4(Tangent, 0.0)));
    vec3 N = normalize(NormalMatrix * Normal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    
    vs_out.TBN = mat3(T, B, N);
    gl_Position = MVP * vec4(Position, 1.0);
}
```

**TBN Matrix (Tangent-Binormal-Normal):**
- **Tangent (T)**: Direction along texture U coordinate
- **Binormal/Bitangent (B)**: Direction along texture V coordinate
- **Normal (N)**: Surface normal direction
- Forms orthonormal basis for tangent space

### Fragment Shader (normal_map.fs)

```glsl
#version 330 core

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoord;
    mat3 TBN;
} fs_in;

uniform vec3 Ka, Kd, Ks;
uniform float Ns;
uniform sampler2D DiffuseMap;
uniform sampler2D NormalMap;
uniform vec3 LightPos;
uniform vec3 ViewPos;
uniform vec3 LightColor;

out vec4 FragColor;

void main()
{
    // Sample and transform normal
    vec3 norm = texture(NormalMap, fs_in.TexCoord).rgb;
    norm = normalize(norm * 2.0 - 1.0);  // Convert from [0,1] to [-1,1]
    norm = normalize(fs_in.TBN * norm); // Transform to world space
    
    vec3 texColor = texture(DiffuseMap, fs_in.TexCoord).rgb;
    
    // ... Rest of Phong calculation with textured normal
}
```

**Normal Map Format:**
- Typically stored in RGB format
- Values range from 0 to 1 (sRGB color space)
- Must be converted to [-1, 1] range: `norm * 2.0 - 1.0`
- X (Red) → Tangent direction
- Y (Green) → Bitangent direction
- Z (Blue) → Normal direction

---

## Unlit Shader

**Files**: `unlit.vs`, `unlit.fs`

### Purpose
Simple shader for debugging and flat color rendering without lighting calculations.

### Vertex Shader (unlit.vs)

```glsl
#version 330 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Color;

uniform mat4 MVP;

out vec3 VertexColor;

void main()
{
    VertexColor = Color;
    gl_Position = MVP * vec4(Position, 1.0);
}
```

### Fragment Shader (unlit.fs)

```glsl
#version 330 core

in vec3 VertexColor;
out vec4 FragColor;

void main()
{
    FragColor = vec4(VertexColor, 1.0);
}
```

**Use Cases:**
- Wireframe rendering
- Debug visualization
- UI elements
- Flat color rendering
- Performance benchmarking

---

## Normal Visualization Shader

**Files**: `normal_visualize.vs`, `normal_visualize.fs`

### Purpose
Visualizes surface normals for debugging and validation.

### How It Works

```glsl
// Fragment shader
void main()
{
    // Normal values range from -1 to 1
    // Convert to 0 to 1 for color display
    vec3 visualNormal = (fs_in.Normal + 1.0) * 0.5;
    FragColor = vec4(visualNormal, 1.0);
}
```

**Color Interpretation:**
- **Red component** (X direction): 0 = -X, 0.5 = 0, 1.0 = +X
- **Green component** (Y direction): 0 = -Y, 0.5 = 0, 1.0 = +Y
- **Blue component** (Z direction): 0 = -Z, 0.5 = 0, 1.0 = +Z

**Expected Colors:**
- Cyan (0, 1, 1): Faces pointing in -X direction
- Red (1, 0, 0): Faces pointing in +X direction
- Magenta (1, 0, 1): Faces pointing in -Y direction
- Green (0, 1, 0): Faces pointing in +Y direction
- Yellow (1, 1, 0): Faces pointing in -Z direction
- Blue (0, 0, 1): Faces pointing in +Z direction
- Light Gray (0.5, 0.5, 0.5): Mixed directions

---

## Uniform Variables

### Transformation Matrices

| Variable | Type | Purpose | Set In |
|----------|------|---------|--------|
| `MVP` | `mat4` | Model-View-Projection matrix | C++ |
| `Model` | `mat4` | Model transformation matrix | C++ |
| `NormalMatrix` | `mat3` | Normal transformation matrix | C++ |

**How to Calculate in C++:**

```cpp
glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
glm::mat4 view = camera.GetViewMatrix();
glm::mat4 projection = camera.GetProjectionMatrix();

glm::mat4 mvp = projection * view * model;
glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));
```

### Material Properties

| Variable | Type | Range | Typical Value |
|----------|------|-------|----------------|
| `Ka` | `vec3` | [0, 1] | 0.2 |
| `Kd` | `vec3` | [0, 1] | 0.8 |
| `Ks` | `vec3` | [0, 1] | 1.0 |
| `Ns` | `float` | [1, 128] | 32.0 |

### Lighting Properties

| Variable | Type | Purpose |
|----------|------|---------|
| `LightPos` | `vec3` | Light position in world space |
| `ViewPos` | `vec3` | Camera position in world space |
| `LightColor` | `vec3` | Light color and intensity |

---

## Vertex Attributes

### Standard Attributes

```glsl
layout (location = 0) in vec3 Position;    // Required for all shaders
layout (location = 1) in vec3 Normal;      // Required for lit shaders
layout (location = 2) in vec2 TexCoord;    // For textured materials
layout (location = 3) in vec3 Tangent;     // For normal mapping
layout (location = 4) in vec3 Bitangent;   // Alternative to calculating B = cross(N, T)
```

**Setting Up VAO in C++:**

```cpp
GLuint VAO, VBO;
glGenVertexArrays(1, &VAO);
glGenBuffers(1, &VBO);

glBindVertexArray(VAO);
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

// Position
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
glEnableVertexAttribArray(0);

// Normal
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
glEnableVertexAttribArray(1);

// TexCoord
glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
glEnableVertexAttribArray(2);

// Tangent
glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
glEnableVertexAttribArray(3);
```

---

## GLSL Basics

### Common Functions

```glsl
// Vector operations
vec3 normalized = normalize(vec);
float length = length(vec);
float dotProduct = dot(vec1, vec2);
vec3 crossProduct = cross(vec1, vec2);

// Math
float clamped = clamp(value, min, max);
float interpolated = mix(value1, value2, t);
float reflected = reflect(incoming, normal);

// Trigonometric
float sine = sin(angle);
float cosine = cos(angle);

// Power
float powered = pow(base, exponent);
float squareRoot = sqrt(value);

// Comparison
vec3 maxVec = max(vec1, vec2);
vec3 minVec = min(vec1, vec2);
float maxComponent = max(vec.x, max(vec.y, vec.z));
```

### Texture Sampling

```glsl
// 2D Texture
vec4 color = texture(sampler2D, vec2 uv);
vec4 colorWithLod = textureLod(sampler2D, vec2 uv, lod);

// Cubemap
vec4 cubeColor = texture(samplerCube, vec3 direction);

// Access components
float red = color.r;    // or color.x
float alpha = color.a;  // or color.w
vec3 rgb = color.rgb;   // or color.xyz
```

### Common Patterns

**Lerp (Linear Interpolation):**
```glsl
vec3 result = mix(colorA, colorB, 0.5);  // 50% blend
```

**Fresnel Effect (Simplified):**
```glsl
float fresnel = pow(1.0 - dot(viewDir, normal), 5.0);
```

**Shadow Mapping:**
```glsl
float shadow = texture(shadowMap, projCoords).r < currentDepth ? 1.0 : 0.0;
```

---

## Troubleshooting

### Common Issues

| Problem | Cause | Solution |
|---------|-------|----------|
| All black | Wrong MVP matrix | Check glUniformMatrix4fv calls |
| Inverted normals | NormalMatrix calculation wrong | Use `transpose(inverse())` |
| No lighting | Missing uniform assignments | Verify all uniform setups in C++ |
| Artifacts on curved surfaces | Interpolated normals not normalized | Add `normalize()` in fragment shader |
| Seams in normal maps | UV wrapping issues | Check texture wrapping mode |

### Debugging Tips

1. Use **Normal Visualization Shader** to validate normals
2. Use **Unlit Shader** to check geometry and colors
3. Check uniform values in appdebugger or RenderDoc
4. Verify shader compilation messages
5. Use `gl_Position = vec4(normal, 1.0)` to visualize normals as position

---

## Performance Tips

1. **Minimize texture lookups** - Each lookup is expensive
2. **Avoid expensive operations in fragment shader** - Move to vertex shader if possible
3. **Use lower precision** - `mediump` for mobile, full `float` for desktop
4. **Pre-compute constants** - Avoid recalculating unchanged values
5. **Use vertex lighting** - When possible, compute in vertex shader and interpolate
6. **Early fragment discard** - Use `discard;` to skip expensive calculations

---

**Last Updated**: 2026
**GLSL Version**: 330 Core
**OpenGL Version**: 3.3+
