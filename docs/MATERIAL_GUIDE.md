# Material Guide - Comprehensive Reference

Complete guide to the CG_HW2 Material system, including usage, properties, and best practices.

## Table of Contents

1. [Material Hierarchy](#material-hierarchy)
2. [Material Classes](#material-classes)
3. [Material Properties](#material-properties)
4. [Creating Materials](#creating-materials)
5. [Material Presets](#material-presets)
6. [Advanced Usage](#advanced-usage)
7. [Material Best Practices](#material-best-practices)

---

## Material Hierarchy

The Material system uses inheritance to provide specialized material types:

```
Material (Base)
├── PhongMaterial
│   └── TexturedMaterial
├── MetallicMaterial
└── DielectricMaterial
```

**Inheritance Benefits:**
- Code reuse through base class functionality
- Polymorphism for generic material handling
- Type-specific features in derived classes

---

## Material Classes

### 1. Material (Base Class)

Abstract base class for all materials.

```cpp
class Material
{
public:
    Material() { name = "Default"; };
    virtual ~Material() {};
    
    void SetName(const std::string mtlName) { name = mtlName; }
    std::string GetName() const { return name; }
    
protected:
    std::string name;
};
```

**Purpose:**
- Defines common interface for all materials
- Enables polymorphic material handling
- Stores material name/identifier

**Usage:**
```cpp
Material* material = new PhongMaterial();
std::string name = material->GetName();
delete material;
```

---

### 2. PhongMaterial

Classic Phong illumination model for realistic lighting with three components.

```cpp
class PhongMaterial : public Material
{
public:
    PhongMaterial();
    
    void SetKa(const glm::vec3 ka);
    void SetKd(const glm::vec3 kd);
    void SetKs(const glm::vec3 ks);
    void SetNs(const float n);
    
    const glm::vec3 GetKa() const;
    const glm::vec3 GetKd() const;
    const glm::vec3 GetKs() const;
    const float GetNs() const;
};
```

**Default Values:**
- Ka = (0.2, 0.2, 0.2) - Ambient
- Kd = (0.8, 0.8, 0.8) - Diffuse
- Ks = (1.0, 1.0, 1.0) - Specular
- Ns = 32.0 - Shininess

**Parameters Explained:**

#### Ka (Ambient Coefficient)
- **Range**: 0.0 to 1.0
- **Effect**: Controls how much ambient light affects the surface
- **Low values** (0.0-0.2): Dark in shadowed areas
- **High values** (0.5-1.0): Bright even in shadows
- **Typical use**: Keep low (0.1-0.3) for realism

#### Kd (Diffuse Coefficient)
- **Range**: 0.0 to 1.0
- **Effect**: Base color and how much diffuse light is reflected
- **Low values** (0.0-0.3): Dark base color
- **High values** (0.7-1.0): Bright base color
- **RGB components**: Can differ for colored surfaces

#### Ks (Specular Coefficient)
- **Range**: 0.0 to 1.0
- **Effect**: Intensity and color of specular highlights
- **Low values** (0.0-0.3): Dull, matte surface
- **High values** (0.7-1.0): Shiny, reflective surface
- **White (1,1,1)**: Colorless highlights (typical for metals)
- **Colored**: Creates colored highlights (rare)

#### Ns (Shininess/Specular Exponent)
- **Range**: 1.0 to 128.0 (higher values possible)
- **Effect**: Size and sharpness of specular highlight
- **Low values** (1-10): Large, diffuse highlight
- **Medium values** (20-50): Normal surface shininess
- **High values** (60-128): Small, sharp highlight (very shiny)

**Complete Example:**

```cpp
PhongMaterial goldMaterial;
goldMaterial.SetName("Gold");

// Gold properties (realistic values)
goldMaterial.SetKa(glm::vec3(0.24725f, 0.1995f, 0.0745f));
goldMaterial.SetKd(glm::vec3(0.75164f, 0.60648f, 0.22648f));
goldMaterial.SetKs(glm::vec3(0.628281f, 0.555802f, 0.366065f));
goldMaterial.SetNs(51.2f);

// Later, in rendering code
shader.Bind();
glUniform3f(locKa, 
    goldMaterial.GetKa().x, 
    goldMaterial.GetKa().y, 
    goldMaterial.GetKa().z);
// ... set other uniforms
```

---

### 3. MetallicMaterial

PBR-inspired material for metallic surfaces with metallic and roughness parameters.

```cpp
class MetallicMaterial : public Material
{
public:
    MetallicMaterial();
    
    void SetKd(const glm::vec3 kd);
    void SetMetallic(const float m);    // 0.0-1.0
    void SetRoughness(const float r);   // 0.0-1.0
    
    const glm::vec3 GetKd() const;
    const float GetMetallic() const;
    const float GetRoughness() const;
};
```

**Default Values:**
- Kd = (0.5, 0.5, 0.5) - Base color
- metallic = 1.0 - Fully metallic
- roughness = 0.2 - Smooth surface

**Parameters Explained:**

#### Kd (Base Color)
- Color of the metallic surface
- Typically metallic tones (gray, copper, gold, etc.)
- Does NOT include specular highlights (automatic based on metallic value)

#### Metallic (0.0 to 1.0)
- **0.0**: Non-metallic (dielectric) surface
- **0.5**: Blend between metal and non-metal
- **1.0**: Fully metallic surface

**Visual Effect:**
- 0.0: Dull, matte appearance
- 0.5: Semi-reflective
- 1.0: Highly reflective, mirror-like

#### Roughness (0.0 to 1.0)
- **0.0**: Perfectly smooth, mirror-like reflections
- **0.5**: Normal surface finish
- **1.0**: Rough, diffuse reflections

**Visual Effect:**
- 0.0: Sharp, focused highlights
- 0.5: Normal texture and highlights
- 1.0: Broad, soft highlights, appears grainy

**Usage Examples:**

```cpp
// Polished Steel
MetallicMaterial steel;
steel.SetKd(glm::vec3(0.77f, 0.77f, 0.77f));
steel.SetMetallic(1.0f);
steel.SetRoughness(0.1f);

// Brushed Aluminum
MetallicMaterial aluminum;
aluminum.SetKd(glm::vec3(0.91f, 0.92f, 0.92f));
aluminum.SetMetallic(1.0f);
aluminum.SetRoughness(0.4f);

// Worn Copper
MetallicMaterial copper;
copper.SetKd(glm::vec3(0.72f, 0.45f, 0.20f));
copper.SetMetallic(0.9f);
copper.SetRoughness(0.3f);
```

---

### 4. DielectricMaterial

Material for insulators (plastics, ceramics, glass) with PBR-style parameters.

```cpp
class DielectricMaterial : public Material
{
public:
    DielectricMaterial();
    
    void SetKd(const glm::vec3 kd);
    void SetKs(const glm::vec3 ks);
    void SetRefractiveIndex(const float ior);
    void SetRoughness(const float r);
    
    const glm::vec3 GetKd() const;
    const glm::vec3 GetKs() const;
    const float GetRefractiveIndex() const;
    const float GetRoughness() const;
};
```

**Default Values:**
- Kd = (0.9, 0.9, 0.9) - Base color
- Ks = (0.5, 0.5, 0.5) - Specular color
- refractiveIndex = 1.5 - IOR for Fresnel
- roughness = 0.1 - Smooth finish

**Parameters Explained:**

#### Kd (Diffuse Color)
- Main color of the dielectric surface
- Examples: plastic color, ceramic color

#### Ks (Specular Color)
- Color of specular highlights
- Usually white or similar to Kd (lower intensity)

#### Refractive Index (IOR)
- Physical property affecting Fresnel effect
- **1.0**: Vacuum/air
- **1.3-1.5**: Common plastics and ceramics
- **1.5**: Glass
- **2.4**: Diamond

**Common IOR Values:**
| Material | IOR |
|----------|-----|
| Air | 1.0 |
| Plastic | 1.3-1.5 |
| Glass | 1.5 |
| Diamond | 2.4 |

#### Roughness (0.0 to 1.0)
- Surface texture and finish
- 0.0 = polished, 1.0 = rough

**Usage Examples:**

```cpp
// Polished Plastic
DielectricMaterial plasticPolished;
plasticPolished.SetKd(glm::vec3(0.95f, 0.95f, 0.95f));
plasticPolished.SetKs(glm::vec3(0.3f, 0.3f, 0.3f));
plasticPolished.SetRefractiveIndex(1.49f);  // ABS plastic
plasticPolished.SetRoughness(0.05f);

// Frosted Glass
DielectricMaterial glassFrosted;
glassFrosted.SetKd(glm::vec3(0.9f, 0.9f, 0.9f));
glassFrosted.SetKs(glm::vec3(0.4f, 0.4f, 0.4f));
glassFrosted.SetRefractiveIndex(1.52f);  // Window glass
glassFrosted.SetRoughness(0.5f);

// Matte Ceramic
DielectricMaterial ceramic;
ceramic.SetKd(glm::vec3(0.8f, 0.7f, 0.6f));
ceramic.SetKs(glm::vec3(0.2f, 0.2f, 0.2f));
ceramic.SetRefractiveIndex(1.46f);  // Ceramic
ceramic.SetRoughness(0.3f);
```

---

### 5. TexturedMaterial

Extends PhongMaterial to support texture maps (diffuse, normal, specular).

```cpp
class TexturedMaterial : public PhongMaterial
{
public:
    TexturedMaterial();
    
    void SetDiffuseTexture(const std::string path);
    void SetNormalMap(const std::string path);
    void SetSpecularTexture(const std::string path);
    
    const std::string GetDiffuseTexture() const;
    const std::string GetNormalMap() const;
    const std::string GetSpecularTexture() const;
};
```

**Supported Textures:**

#### Diffuse Map (Albedo)
- **Purpose**: Base color/albedo of the surface
- **Format**: RGB or RGBA
- **Example**: Wood grain, fabric weave, brick pattern
- **Usage**: Modulates Kd coefficient

#### Normal Map
- **Purpose**: Adds surface detail without geometry
- **Format**: RGB (often with alpha for height/ao)
- **Convention**: 
  - Red (X) = Tangent direction
  - Green (Y) = Bitangent direction
  - Blue (Z) = Normal direction
  - Neutral color (0.5, 0.5, 1.0) = flat normal
- **Example**: Stone, leather, fabric details

#### Specular Map
- **Purpose**: Controls highlight intensity per-pixel
- **Format**: Grayscale or RGB
- **Example**: Worn areas have lower specularity

**Usage Example:**

```cpp
TexturedMaterial brickMaterial;
brickMaterial.SetName("Brick Wall");

// Inherit Phong properties
brickMaterial.SetKd(glm::vec3(0.8f, 0.8f, 0.8f));
brickMaterial.SetKs(glm::vec3(0.2f, 0.2f, 0.2f));
brickMaterial.SetNs(16.0f);

// Set textures
brickMaterial.SetDiffuseTexture("textures/brick_diffuse.png");
brickMaterial.SetNormalMap("textures/brick_normal.png");
brickMaterial.SetSpecularTexture("textures/brick_specular.png");
```

**Shader Integration:**

```cpp
// In C++ rendering code
auto texMat = dynamic_cast<TexturedMaterial*>(material);
if (texMat) {
    // Load textures
    GLuint diffuseTex = LoadTexture(texMat->GetDiffuseTexture());
    GLuint normalTex = LoadTexture(texMat->GetNormalMap());
    
    // Bind to texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseTex);
    glUniform1i(locDiffuseMap, 0);
    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, normalTex);
    glUniform1i(locNormalMap, 1);
}
```

---

## Material Properties

### Property Ranges and Guidelines

| Property | Min | Max | Default | Recommendation |
|----------|-----|-----|---------|-----------------|
| Ka (ambient) | 0.0 | 1.0 | 0.2 | 0.1-0.3 |
| Kd (diffuse) | 0.0 | 1.0 | 0.8 | 0.5-0.95 |
| Ks (specular) | 0.0 | 1.0 | 1.0 | 0.2-1.0 |
| Ns (shininess) | 1.0 | 128+ | 32.0 | 10-100 |
| metallic | 0.0 | 1.0 | 0.5 | Discrete |
| roughness | 0.0 | 1.0 | 0.5 | 0.05-0.95 |
| IOR | 1.0 | 3.0+ | 1.5 | Material-dependent |

---

## Creating Materials

### Basic Material Creation

```cpp
// Create a material
PhongMaterial mat;

// Set properties
mat.SetName("Custom Material");
mat.SetKa(glm::vec3(0.1f, 0.1f, 0.1f));
mat.SetKd(glm::vec3(0.7f, 0.7f, 0.7f));
mat.SetKs(glm::vec3(1.0f, 1.0f, 1.0f));
mat.SetNs(64.0f);

// Use in rendering
shader.Bind();
glUniform3f(locKa, mat.GetKa().x, mat.GetKa().y, mat.GetKa().z);
glUniform3f(locKd, mat.GetKd().x, mat.GetKd().y, mat.GetKd().z);
glUniform3f(locKs, mat.GetKs().x, mat.GetKs().y, mat.GetKs().z);
glUniform1f(locNs, mat.GetNs());
// Render geometry
```

### Material Storage and Reuse

```cpp
// Store materials for reuse
std::map<std::string, PhongMaterial> materials;

// Create and store
PhongMaterial goldMat;
goldMat.SetName("Gold");
goldMat.SetKa(glm::vec3(0.24725f, 0.1995f, 0.0745f));
goldMat.SetKd(glm::vec3(0.75164f, 0.60648f, 0.22648f));
goldMat.SetKs(glm::vec3(0.628281f, 0.555802f, 0.366065f));
goldMat.SetNs(51.2f);

materials["gold"] = goldMat;

// Later: retrieve and use
PhongMaterial& mat = materials["gold"];
// ... use material
```

### Creating Custom Material Classes

```cpp
class GlassMaterial : public PhongMaterial
{
public:
    GlassMaterial() {
        SetKa(glm::vec3(0.05f, 0.05f, 0.05f));
        SetKd(glm::vec3(0.9f, 0.9f, 0.9f));
        SetKs(glm::vec3(1.0f, 1.0f, 1.0f));
        SetNs(100.0f);
        transparency = 0.8f;
    }
    
    void SetTransparency(float t) { transparency = glm::clamp(t, 0.0f, 1.0f); }
    float GetTransparency() const { return transparency; }
    
private:
    float transparency;
};
```

---

## Material Presets

Predefined material configurations for common materials.

### Metals

#### Polished Gold
```cpp
PhongMaterial gold;
gold.SetKa(glm::vec3(0.24725f, 0.1995f, 0.0745f));
gold.SetKd(glm::vec3(0.75164f, 0.60648f, 0.22648f));
gold.SetKs(glm::vec3(0.628281f, 0.555802f, 0.366065f));
gold.SetNs(51.2f);
```

#### Polished Silver
```cpp
PhongMaterial silver;
silver.SetKa(glm::vec3(0.23125f, 0.23125f, 0.23125f));
silver.SetKd(glm::vec3(0.2775f, 0.2775f, 0.2775f));
silver.SetKs(glm::vec3(0.773911f, 0.773911f, 0.773911f));
silver.SetNs(89.6f);
```

#### Polished Copper
```cpp
PhongMaterial copper;
copper.SetKa(glm::vec3(0.2295f, 0.08825f, 0.0275f));
copper.SetKd(glm::vec3(0.7038f, 0.27048f, 0.0828f));
copper.SetKs(glm::vec3(0.256777f, 0.137622f, 0.086014f));
copper.SetNs(12.8f);
```

### Plastics

#### Red Plastic
```cpp
PhongMaterial redPlastic;
redPlastic.SetKa(glm::vec3(0.0f, 0.0f, 0.0f));
redPlastic.SetKd(glm::vec3(0.5f, 0.0f, 0.0f));
redPlastic.SetKs(glm::vec3(0.7f, 0.6f, 0.6f));
redPlastic.SetNs(32.0f);
```

#### White Plastic
```cpp
PhongMaterial whitePlastic;
whitePlastic.SetKa(glm::vec3(0.0f, 0.0f, 0.0f));
whitePlastic.SetKd(glm::vec3(1.0f, 1.0f, 1.0f));
whitePlastic.SetKs(glm::vec3(0.7f, 0.7f, 0.7f));
whitePlastic.SetNs(10.0f);
```

#### Black Plastic
```cpp
PhongMaterial blackPlastic;
blackPlastic.SetKa(glm::vec3(0.0f, 0.0f, 0.0f));
blackPlastic.SetKd(glm::vec3(0.01f, 0.01f, 0.01f));
blackPlastic.SetKs(glm::vec3(0.5f, 0.5f, 0.5f));
blackPlastic.SetNs(32.0f);
```

### Natural Materials

#### Wood
```cpp
PhongMaterial wood;
wood.SetKa(glm::vec3(0.17f, 0.09f, 0.03f));
wood.SetKd(glm::vec3(0.78f, 0.57f, 0.11f));
wood.SetKs(glm::vec3(0.0f, 0.0f, 0.0f));
wood.SetNs(11.0f);
```

#### Rubber
```cpp
PhongMaterial rubber;
rubber.SetKa(glm::vec3(0.02f, 0.02f, 0.02f));
rubber.SetKd(glm::vec3(0.01f, 0.01f, 0.01f));
rubber.SetKs(glm::vec3(0.04f, 0.04f, 0.04f));
rubber.SetNs(10.0f);
```

---

## Advanced Usage

### Dynamic Material Switching

```cpp
std::vector<PhongMaterial> materials;
int currentMaterialIndex = 0;

void SwitchMaterial(int index) {
    if (index >= 0 && index < materials.size()) {
        currentMaterialIndex = index;
    }
}

void Render() {
    PhongMaterial& mat = materials[currentMaterialIndex];
    // Apply material uniforms and render
}
```

### Interpolating Between Materials

```cpp
PhongMaterial LerpMaterial(const PhongMaterial& a, const PhongMaterial& b, float t) {
    PhongMaterial result;
    result.SetKa(glm::mix(a.GetKa(), b.GetKa(), t));
    result.SetKd(glm::mix(a.GetKd(), b.GetKd(), t));
    result.SetKs(glm::mix(a.GetKs(), b.GetKs(), t));
    result.SetNs(glm::mix(a.GetNs(), b.GetNs(), t));
    return result;
}
```

### Material Factory Pattern

```cpp
class MaterialFactory {
public:
    static PhongMaterial CreateGold() { /* ... */ }
    static PhongMaterial CreateSilver() { /* ... */ }
    static PhongMaterial CreateRedPlastic() { /* ... */ }
    // ... more presets
};
```

---

## Material Best Practices

### 1. **Use Realistic Values**
- Reference real materials for K values
- Don't exceed 1.0 for physical accuracy

### 2. **Maintain Material Consistency**
- Keep ambient Ka low (0.1-0.3)
- Diffuse Kd usually higher than Ka
- Metallic objects: Kd colors metallic hues, Ks is white

### 3. **Test with Different Lighting**
- Materials look different under various lighting conditions
- Test with multiple light colors and positions

### 4. **Optimize for Performance**
- Cache frequently used materials
- Batch render objects with same material
- Minimize material switches per frame

### 5. **Use Textures Wisely**
- High-resolution textures for close-up viewing
- Normal maps for geometry detail
- Specular maps to break up uniformity

### 6. **Document Custom Materials**
```cpp
// Gold Material
// Based on real-world gold properties
// Ka: 0.247, Kd: 0.752, Ks: 0.628, Ns: 51.2
// Suitable for jewelry and luxury items
PhongMaterial gold;
// ...
```

---

**Last Updated**: 2026
**Version**: 1.0.0
