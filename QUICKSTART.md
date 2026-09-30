# Quick Start Guide - CG_HW2 Shader & Material System

Get up and running with the CG_HW2 Shader and Material system in 5 minutes.

## Prerequisites

- OpenGL 3.3+ compatible graphics card
- C++11 or higher compiler
- GLM library (for math operations)
- GLFW or similar for window management
- GLEW or similar for OpenGL function loading

## Installation

1. **Copy Files to Your Project**
   ```
   Your Project/
   ├── material.h
   ├── shaderprog.h
   ├── shaderprog.cpp
   └── shaders/
       ├── vis_material.vs
       └── vis_material.fs
   ```

2. **Include Headers**
   ```cpp
   #include "material.h"
   #include "shaderprog.h"
   ```

3. **Link Libraries**
   ```cmake
   # CMakeLists.txt example
   target_link_libraries(your_project OpenGL::GL)
   target_include_directories(your_project PRIVATE ${GLM_INCLUDE_DIR})
   ```

## Basic Usage (3 Steps)

### Step 1: Create a Material

```cpp
PhongMaterial material;
material.SetName("My Material");
material.SetKd(glm::vec3(0.8f, 0.8f, 0.8f));  // Gray color
material.SetKs(glm::vec3(1.0f, 1.0f, 1.0f));  // Shiny
material.SetNs(32.0f);                        // Medium shine
```

### Step 2: Load Shader Program

```cpp
VisMaterialShaderProg shader;
shader.LoadFromFiles("shaders/vis_material.vs", "shaders/vis_material.fs");
```

### Step 3: Render with Material

```cpp
shader.Bind();

// Set transformation matrices
glm::mat4 mvp = projection * view * model;
glUniformMatrix4fv(shader.GetLocMVP(), 1, GL_FALSE, glm::value_ptr(mvp));

// Set material
GLint locKd = glGetUniformLocation(shader.shaderProgId, "Kd");
glUniform3f(locKd, material.GetKd().x, material.GetKd().y, material.GetKd().z);

// Set lighting
GLint locLightPos = glGetUniformLocation(shader.shaderProgId, "LightPos");
glUniform3f(locLightPos, 5.0f, 5.0f, 5.0f);

// Render your geometry
glDrawArrays(GL_TRIANGLES, 0, vertexCount);

shader.UnBind();
```

## Common Materials

### Metal Surface

```cpp
PhongMaterial metal;
metal.SetKd(glm::vec3(0.7f, 0.7f, 0.7f));     // Light gray
metal.SetKs(glm::vec3(1.0f, 1.0f, 1.0f));     // White highlights
metal.SetNs(100.0f);                          // Very shiny
```

### Plastic Surface

```cpp
PhongMaterial plastic;
plastic.SetKd(glm::vec3(0.5f, 0.0f, 0.0f));   // Red
plastic.SetKs(glm::vec3(0.7f, 0.6f, 0.6f));   // Slightly shiny
plastic.SetNs(32.0f);                         // Normal shine
```

### Matte Surface

```cpp
PhongMaterial matte;
matte.SetKd(glm::vec3(0.6f, 0.6f, 0.6f));     // Dark gray
matte.SetKs(glm::vec3(0.0f, 0.0f, 0.0f));     // No shine
matte.SetNs(1.0f);                            // Very dull
```

## Material Parameter Guide

### Ka (Ambient)
- Low values (0.1-0.3): More realistic, darker shadows
- High values (0.5-1.0): Brighter in shadows
- **Typical**: 0.2

### Kd (Diffuse/Color)
- Controls the base color and brightness
- **Example**: Red = (1, 0, 0), Green = (0, 1, 0), Gray = (0.5, 0.5, 0.5)
- **Typical**: 0.5-0.95

### Ks (Specular)
- Controls highlight shine
- White (1, 1, 1): Colorless highlights
- Dark/Black (0, 0, 0): No highlights (matte)
- **Typical**: 0.5-1.0

### Ns (Shininess)
- 1-10: Large, diffuse highlights (dull)
- 20-50: Normal surfaces
- 60-128: Shiny, mirror-like
- **Typical**: 32

## Debugging

### Check Material Properties

```cpp
std::cout << "Material: " << material.GetName() << std::endl;
std::cout << "Kd: (" << material.GetKd().x << ", " 
                      << material.GetKd().y << ", " 
                      << material.GetKd().z << ")" << std::endl;
std::cout << "Ns: " << material.GetNs() << std::endl;
```

### Use Unlit Shader (No Lighting)

```cpp
ShaderProg unlitShader;
unlitShader.LoadFromFiles("shaders/unlit.vs", "shaders/unlit.fs");
// Use this to verify geometry is correct without lighting complexity
```

### Visualize Normals

```cpp
ShaderProg normalShader;
normalShader.LoadFromFiles("shaders/normal_visualize.vs", "shaders/normal_visualize.fs");
// Colors show normal directions for debugging
```

## Common Issues & Solutions

### Problem: All Black Geometry

**Cause**: 
- Normals pointing wrong direction
- Light position incorrect
- Ka value too low

**Solution**:
```cpp
// Increase ambient
material.SetKa(glm::vec3(0.3f, 0.3f, 0.3f));

// Or check light position
glUniform3f(locLightPos, 0.0f, 5.0f, 5.0f);  // Move light to reasonable position
```

### Problem: Completely White Geometry

**Cause**:
- All material values set to 1.0
- Light color too bright

**Solution**:
```cpp
// Reduce material properties
material.SetKd(glm::vec3(0.8f, 0.8f, 0.8f));  // Was 1.0

// Or reduce light intensity
glUniform3f(locLightColor, 0.7f, 0.7f, 0.7f);  // Dim the light
```

### Problem: No Highlights/Shine

**Cause**:
- Ks set to 0
- Ns too low
- Specular light color dark

**Solution**:
```cpp
// Enable specular
material.SetKs(glm::vec3(1.0f, 1.0f, 1.0f));
material.SetNs(50.0f);  // Higher = sharper highlight
```

### Problem: Shader Compilation Error

**Cause**:
- Wrong GLSL version
- Missing shader files
- Syntax error in shader

**Solution**:
```cpp
// Check if file exists
std::ifstream file("shaders/vis_material.vs");
if (!file.good()) {
    std::cerr << "Shader file not found!" << std::endl;
}

// Check OpenGL version
std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
```

## Next Steps

1. **Read the Full Documentation**
   - See [README.md](README.md) for complete overview
   - See [docs/SHADER_GUIDE.md](docs/SHADER_GUIDE.md) for shader details
   - See [docs/MATERIAL_GUIDE.md](docs/MATERIAL_GUIDE.md) for material reference

2. **Explore Examples**
   - See [USAGE_EXAMPLES.cpp](USAGE_EXAMPLES.cpp) for more examples
   - Look at how materials are created and used

3. **Create Custom Materials**
   - Extend `PhongMaterial` class
   - Experiment with different K values
   - Create material presets for your project

4. **Advanced Topics**
   - Implement metallic/dielectric materials
   - Add texture mapping
   - Create custom shaders

## Performance Tips

1. **Batch materials**: Minimize shader switches per frame
2. **Cache uniforms**: Store uniform locations, don't query every frame
3. **Reuse materials**: Store frequently used materials
4. **Use mipmaps**: For textured materials
5. **Reduce texture size**: For less VRAM usage

## Integration with Scene Manager

```cpp
class Scene {
private:
    std::map<std::string, PhongMaterial> materials;
    VisMaterialShaderProg shader;

public:
    void Initialize() {
        shader.LoadFromFiles("shaders/vis_material.vs", 
                            "shaders/vis_material.fs");
        
        // Create default materials
        PhongMaterial defaultMat;
        defaultMat.SetName("Default");
        materials["default"] = defaultMat;
    }

    void Render(const std::string& materialName, 
                const glm::mat4& model, 
                GLuint vao, int count) {
        if (materials.find(materialName) == materials.end())
            return;
        
        PhongMaterial& mat = materials[materialName];
        // Render with this material
    }
};
```

## Troubleshooting Checklist

- [ ] OpenGL context initialized properly
- [ ] Shader files exist in correct path
- [ ] VAO/VBO setup with correct vertex layout
- [ ] Normals included in vertex data
- [ ] MVP matrix calculated correctly
- [ ] Material Kd values are not all zero
- [ ] Light position is reasonable
- [ ] NormalMatrix calculated as transpose(inverse(Model))
- [ ] All uniform values set before rendering
- [ ] Shader program bound before setting uniforms

## Getting Help

1. Check [docs/TECHNICAL.md](docs/TECHNICAL.md) for technical details
2. Review shader source in `shaders/` directory
3. Check OpenGL error log: `glGetError()`
4. Verify shader compilation: `glGetShaderInfoLog()`
5. Test with normal visualization shader

---

**Happy Rendering!** 🎨

**Last Updated**: 2026
