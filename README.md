# CG_HW2 Shader & Material System

A comprehensive OpenGL-based Shader and Material system for computer graphics applications. This system provides flexible material definitions and modern shader implementations for 3D rendering.

## Overview

This project implements a modular shader and material system with support for:

- **Phong Lighting Model**: Classic illumination model for realistic lighting
- **Normal Mapping**: Advanced surface detail through normal maps
- **Metallic & Dielectric Materials**: PBR-inspired material types
- **Textured Materials**: Support for diffuse, normal, and specular maps
- **Debug Visualizations**: Normal visualization for development

## Project Structure

```
CG_HW2/
├── material.h              # Material class definitions
├── shaderprog.h            # Shader program classes
├── shaderprog.cpp          # Shader program implementations
├── shaders/                # Shader files
│   ├── vis_material.vs     # Phong lighting vertex shader
│   ├── vis_material.fs     # Phong lighting fragment shader
│   ├── unlit.vs            # Unlit debug vertex shader
│   ├── unlit.fs            # Unlit debug fragment shader
│   ├── normal_map.vs       # Normal mapping vertex shader
│   ├── normal_map.fs       # Normal mapping fragment shader
│   ├── normal_visualize.vs # Normal visualization vertex shader
│   └── normal_visualize.fs # Normal visualization fragment shader
├── camera.h/cpp            # Camera implementation
├── trianglemesh.h/cpp      # Mesh data structures
└── docs/                   # Documentation
    ├── SHADER_GUIDE.md     # Detailed shader guide
    ├── MATERIAL_GUIDE.md   # Material system guide
    └── TECHNICAL.md        # Technical specifications
```

## Material Types

### 1. PhongMaterial
Classic Phong lighting model with ambient, diffuse, and specular components.

```cpp
PhongMaterial mat;
mat.SetKa(glm::vec3(0.2f, 0.2f, 0.2f));  // Ambient color
mat.SetKd(glm::vec3(0.8f, 0.8f, 0.8f));  // Diffuse color
mat.SetKs(glm::vec3(1.0f, 1.0f, 1.0f));  // Specular color
mat.SetNs(32.0f);                        // Shininess (higher = shinier)
```

**Parameters:**
- `Ka`: Ambient reflection coefficient (0.0-1.0)
- `Kd`: Diffuse reflection coefficient (0.0-1.0)
- `Ks`: Specular reflection coefficient (0.0-1.0)
- `Ns`: Shininess exponent (typical range: 1.0-128.0)

### 2. MetallicMaterial
PBR-inspired material for metallic surfaces.

```cpp
MetallicMaterial mat;
mat.SetKd(glm::vec3(0.5f, 0.5f, 0.5f));  // Base color
mat.SetMetallic(1.0f);                   // Full metallic (0.0-1.0)
mat.SetRoughness(0.2f);                  // Smooth surface (0.0-1.0)
```

**Parameters:**
- `Kd`: Base color
- `metallic`: How metallic the surface is (0.0 = dielectric, 1.0 = metal)
- `roughness`: Surface roughness (0.0 = smooth, 1.0 = rough)

### 3. DielectricMaterial
Material for insulators like plastic, ceramic, or glass.

```cpp
DielectricMaterial mat;
mat.SetKd(glm::vec3(0.9f, 0.9f, 0.9f));  // Base color
mat.SetKs(glm::vec3(0.5f, 0.5f, 0.5f));  // Specular color
mat.SetRefractiveIndex(1.5f);            // IOR for Fresnel
mat.SetRoughness(0.1f);                  // Surface finish
```

**Parameters:**
- `Kd`: Diffuse reflection
- `Ks`: Specular reflection
- `refractiveIndex`: Refractive index for Fresnel calculations
- `roughness`: Surface roughness

### 4. TexturedMaterial
Extends PhongMaterial with texture support.

```cpp
TexturedMaterial mat;
mat.SetDiffuseTexture("textures/diffuse.png");
mat.SetNormalMap("textures/normal.png");
mat.SetSpecularTexture("textures/specular.png");
```

**Supported Textures:**
- Diffuse map (color/albedo)
- Normal map (surface detail)
- Specular map (reflection intensity)

## Shader Programs

### vis_material.vs / vis_material.fs
Complete Phong lighting shader with full illumination model.

**Features:**
- Full Phong lighting calculation
- Normal transformation (NormalMatrix)
- Per-fragment illumination
- Configurable light and material properties

**Uniforms:**
```glsl
uniform mat4 MVP;           // Model-View-Projection matrix
uniform mat4 Model;         // Model matrix
uniform mat3 NormalMatrix;  // Normal matrix (inverse transpose of Model)
uniform vec3 Ka;            // Ambient coefficient
uniform vec3 Kd;            // Diffuse coefficient
uniform vec3 Ks;            // Specular coefficient
uniform float Ns;           // Shininess
uniform vec3 LightPos;      // Light position in world space
uniform vec3 ViewPos;       // Camera position
uniform vec3 LightColor;    // Light color and intensity
```

### unlit.vs / unlit.fs
Simple unlit shader for debugging and flat color visualization.

**Use Cases:**
- Debug rendering without lighting
- Wireframe visualization
- UI overlays

### normal_map.vs / normal_map.fs
Advanced shader with normal mapping support.

**Features:**
- Tangent space normal mapping
- Automatic TBN matrix generation
- Texture sampling (diffuse, normal, specular)
- Phong-style illumination with normal maps

**Vertex Attributes:**
- Position
- Normal
- TexCoord (UV coordinates)
- Tangent (for TBN generation)

### normal_visualize.vs / normal_visualize.fs
Visualizes surface normals for debugging.

**Color Mapping:**
- X direction → Red channel
- Y direction → Green channel
- Z direction → Blue channel

## Usage Example

```cpp
// Create a material
PhongMaterial goldMaterial;
goldMaterial.SetName("Gold");
goldMaterial.SetKa(glm::vec3(0.24725f, 0.1995f, 0.0745f));
goldMaterial.SetKd(glm::vec3(0.75164f, 0.60648f, 0.22648f));
goldMaterial.SetKs(glm::vec3(0.628281f, 0.555802f, 0.366065f));
goldMaterial.SetNs(51.2f);

// Load shader program
VisMaterialShaderProg shader;
shader.LoadFromFiles("shaders/vis_material.vs", "shaders/vis_material.fs");

// Bind and render
shader.Bind();
// Set uniforms based on material and lighting
// Render mesh geometry
shader.UnBind();
```

## Shader Compilation

Shaders are compiled at runtime by the `ShaderProg` class:

```cpp
ShaderProg shader;
bool success = shader.LoadFromFiles("path/to/vertex.vs", "path/to/fragment.fs");
if (success) {
    shader.Bind();
    // Render
    shader.UnBind();
}
```

## Creating Custom Materials

Extend the `Material` base class:

```cpp
class CustomMaterial : public Material
{
public:
    CustomMaterial() {
        name = "Custom";
        // Initialize properties
    }
    
    void SetCustomProperty(float value) { customProperty = value; }
    float GetCustomProperty() const { return customProperty; }
    
private:
    float customProperty;
};
```

## Creating Custom Shaders

1. Create `.vs` (vertex) and `.fs` (fragment) files in the `shaders/` directory
2. Extend `ShaderProg` class if custom uniform handling is needed:

```cpp
class CustomShaderProg : public ShaderProg
{
public:
    CustomShaderProg() {};
    ~CustomShaderProg() {};
    
    GLint GetLocCustomUniform() const { return locCustom; }
    
protected:
    void GetUniformVariableLocation() override;
    
private:
    GLint locCustom;
};
```

## Common Material Presets

### Metals
- **Steel**: Ka(0.25), Kd(0.25), Ks(0.77), Ns(89.6)
- **Copper**: Ka(0.19, 0.07, 0.02), Kd(0.70, 0.27, 0.11), Ks(0.92, 0.81, 0.77), Ns(27.9)
- **Gold**: Ka(0.247, 0.199, 0.075), Kd(0.752, 0.606, 0.226), Ks(0.628, 0.556, 0.366), Ns(51.2)

### Plastics
- **Red Plastic**: Ka(0.0, 0.0, 0.0), Kd(0.5, 0.0, 0.0), Ks(0.7, 0.6, 0.6), Ns(32)
- **White Plastic**: Ka(0.0, 0.0, 0.0), Kd(1.0, 1.0, 1.0), Ks(0.7, 0.7, 0.7), Ns(10)

### Natural Materials
- **Wood**: Ka(0.17, 0.09, 0.03), Kd(0.78, 0.57, 0.11), Ks(0.0, 0.0, 0.0), Ns(11)
- **Rubber**: Ka(0.02, 0.02, 0.02), Kd(0.01, 0.01, 0.01), Ks(0.04, 0.04, 0.04), Ns(10)

## Performance Considerations

1. **Fragment Shader Complexity**: Move heavy calculations to vertex shader when possible
2. **Texture Lookup**: Minimize texture samples in fragment shader
3. **Normal Calculation**: Pre-compute normals when static
4. **Material Count**: Use material arrays/buffers for many materials

## Requirements

- OpenGL 3.3 or higher
- GLSL 330 or compatible
- Support for:
  - Vertex Attributes (layout locations)
  - Uniform Blocks (optional, for optimization)
  - Texture Sampling

## Future Enhancements

- [ ] PBR (Physically Based Rendering) implementation
- [ ] Parallax mapping
- [ ] Shadow mapping
- [ ] Ambient occlusion
- [ ] Environment mapping (cubemaps)
- [ ] Material serialization (JSON/XML)
- [ ] Real-time material preview UI

## References

- [LearnOpenGL - Lighting](https://learnopengl.com/Lighting/Basic-Lighting)
- [LearnOpenGL - Normal Mapping](https://learnopengl.com/Advanced-Lighting/Normal-Mapping)
- [Phong Reflection Model](https://en.wikipedia.org/wiki/Phong_reflection_model)
- [PBR Introduction](https://www.marmoset.co/posts/basic-theory-of-physically-based-rendering/)

## Support

For issues and questions, please refer to the technical documentation in the `docs/` folder.

---

**License**: MIT License (see LICENSE file)

**Version**: 1.0.0

**Last Updated**: 2026
