/**
 * CG_HW2 Shader & Material System - Usage Example
 * 
 * This file demonstrates how to use the Shader and Material system
 * for rendering 3D objects with proper materials and lighting.
 */

#include "material.h"
#include "shaderprog.h"
#include "camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

/**
 * Example 1: Using Phong Material with Basic Lighting
 */
void Example_BasicPhongLighting()
{
    // Step 1: Create a material
    PhongMaterial goldMaterial;
    goldMaterial.SetName("Gold");
    goldMaterial.SetKa(glm::vec3(0.24725f, 0.1995f, 0.0745f));    // Ambient
    goldMaterial.SetKd(glm::vec3(0.75164f, 0.60648f, 0.22648f));   // Diffuse
    goldMaterial.SetKs(glm::vec3(0.628281f, 0.555802f, 0.366065f)); // Specular
    goldMaterial.SetNs(51.2f);                                      // Shininess

    // Step 2: Load shader program
    VisMaterialShaderProg shader;
    if (!shader.LoadFromFiles("shaders/vis_material.vs", "shaders/vis_material.fs")) {
        // Handle shader compilation error
        return;
    }

    // Step 3: Prepare rendering (assuming VAO/VBO already set up)
    // (In real code, you would bind VAO, VBO here)

    // Step 4: In render loop
    shader.Bind();

    // Set transformation matrices
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);  // Get from camera
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    glm::mat4 mvp = projection * view * model;

    // Calculate normal matrix for transforming normals
    glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));

    // Set matrix uniforms
    GLint locMVP = shader.GetLocMVP();
    GLint locModel = glGetUniformLocation(shader.shaderProgId, "Model");
    GLint locNormalMatrix = glGetUniformLocation(shader.shaderProgId, "NormalMatrix");

    glUniformMatrix4fv(locMVP, 1, GL_FALSE, glm::value_ptr(mvp));
    glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix3fv(locNormalMatrix, 1, GL_FALSE, glm::value_ptr(normalMatrix));

    // Set material uniforms
    GLint locKa = glGetUniformLocation(shader.shaderProgId, "Ka");
    GLint locKd = glGetUniformLocation(shader.shaderProgId, "Kd");
    GLint locKs = glGetUniformLocation(shader.shaderProgId, "Ks");
    GLint locNs = glGetUniformLocation(shader.shaderProgId, "Ns");

    glUniform3f(locKa, goldMaterial.GetKa().x, goldMaterial.GetKa().y, goldMaterial.GetKa().z);
    glUniform3f(locKd, goldMaterial.GetKd().x, goldMaterial.GetKd().y, goldMaterial.GetKd().z);
    glUniform3f(locKs, goldMaterial.GetKs().x, goldMaterial.GetKs().y, goldMaterial.GetKs().z);
    glUniform1f(locNs, goldMaterial.GetNs());

    // Set lighting uniforms
    glm::vec3 lightPos(5.0f, 5.0f, 5.0f);
    glm::vec3 viewPos(0.0f, 0.0f, 10.0f);
    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

    GLint locLightPos = glGetUniformLocation(shader.shaderProgId, "LightPos");
    GLint locViewPos = glGetUniformLocation(shader.shaderProgId, "ViewPos");
    GLint locLightColor = glGetUniformLocation(shader.shaderProgId, "LightColor");

    glUniform3f(locLightPos, lightPos.x, lightPos.y, lightPos.z);
    glUniform3f(locViewPos, viewPos.x, viewPos.y, viewPos.z);
    glUniform3f(locLightColor, lightColor.x, lightColor.y, lightColor.z);

    // Render geometry
    // glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    // or
    // glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    shader.UnBind();
}

/**
 * Example 2: Multiple Materials
 */
void Example_MultipleMaterials()
{
    // Create a material library
    std::vector<PhongMaterial> materials;

    // Gold
    PhongMaterial gold;
    gold.SetName("Gold");
    gold.SetKa(glm::vec3(0.24725f, 0.1995f, 0.0745f));
    gold.SetKd(glm::vec3(0.75164f, 0.60648f, 0.22648f));
    gold.SetKs(glm::vec3(0.628281f, 0.555802f, 0.366065f));
    gold.SetNs(51.2f);
    materials.push_back(gold);

    // Silver
    PhongMaterial silver;
    silver.SetName("Silver");
    silver.SetKa(glm::vec3(0.23125f, 0.23125f, 0.23125f));
    silver.SetKd(glm::vec3(0.2775f, 0.2775f, 0.2775f));
    silver.SetKs(glm::vec3(0.773911f, 0.773911f, 0.773911f));
    silver.SetNs(89.6f);
    materials.push_back(silver);

    // Red Plastic
    PhongMaterial redPlastic;
    redPlastic.SetName("Red Plastic");
    redPlastic.SetKa(glm::vec3(0.0f, 0.0f, 0.0f));
    redPlastic.SetKd(glm::vec3(0.5f, 0.0f, 0.0f));
    redPlastic.SetKs(glm::vec3(0.7f, 0.6f, 0.6f));
    redPlastic.SetNs(32.0f);
    materials.push_back(redPlastic);

    // Use materials in rendering loop
    for (int i = 0; i < materials.size(); ++i) {
        PhongMaterial& mat = materials[i];
        // Render object with this material
        // Set uniforms, render geometry
    }
}

/**
 * Example 3: Metallic Material
 */
void Example_MetallicMaterial()
{
    // Polished Steel
    MetallicMaterial steel;
    steel.SetName("Polished Steel");
    steel.SetKd(glm::vec3(0.77f, 0.77f, 0.77f));
    steel.SetMetallic(1.0f);      // Fully metallic
    steel.SetRoughness(0.1f);     // Smooth/polished

    // Brushed Aluminum
    MetallicMaterial aluminum;
    aluminum.SetName("Brushed Aluminum");
    aluminum.SetKd(glm::vec3(0.91f, 0.92f, 0.92f));
    aluminum.SetMetallic(1.0f);
    aluminum.SetRoughness(0.4f);  // Rough/brushed

    // Note: Metallic materials require a specialized shader
    // that uses the metallic and roughness properties
    // The standard vis_material shader doesn't support these
}

/**
 * Example 4: Dielectric Material
 */
void Example_DielectricMaterial()
{
    // Polished Plastic
    DielectricMaterial plasticPolished;
    plasticPolished.SetName("Polished Plastic");
    plasticPolished.SetKd(glm::vec3(0.95f, 0.95f, 0.95f));
    plasticPolished.SetKs(glm::vec3(0.3f, 0.3f, 0.3f));
    plasticPolished.SetRefractiveIndex(1.49f);  // ABS plastic
    plasticPolished.SetRoughness(0.05f);

    // Frosted Glass
    DielectricMaterial glassFrosted;
    glassFrosted.SetName("Frosted Glass");
    glassFrosted.SetKd(glm::vec3(0.9f, 0.9f, 0.9f));
    glassFrosted.SetKs(glm::vec3(0.4f, 0.4f, 0.4f));
    glassFrosted.SetRefractiveIndex(1.52f);  // Window glass
    glassFrosted.SetRoughness(0.5f);

    // Matte Ceramic
    DielectricMaterial ceramic;
    ceramic.SetName("Matte Ceramic");
    ceramic.SetKd(glm::vec3(0.8f, 0.7f, 0.6f));
    ceramic.SetKs(glm::vec3(0.2f, 0.2f, 0.2f));
    ceramic.SetRefractiveIndex(1.46f);
    ceramic.SetRoughness(0.3f);
}

/**
 * Example 5: Textured Material
 */
void Example_TexturedMaterial()
{
    TexturedMaterial brickWall;
    brickWall.SetName("Brick Wall");

    // Set Phong properties
    brickWall.SetKd(glm::vec3(0.8f, 0.8f, 0.8f));
    brickWall.SetKs(glm::vec3(0.2f, 0.2f, 0.2f));
    brickWall.SetNs(16.0f);

    // Set texture paths
    brickWall.SetDiffuseTexture("textures/brick_diffuse.png");
    brickWall.SetNormalMap("textures/brick_normal.png");
    brickWall.SetSpecularTexture("textures/brick_specular.png");

    // In rendering code, check if material has textures and load them
    std::string diffusePath = brickWall.GetDiffuseTexture();
    if (!diffusePath.empty()) {
        // Load texture: GLuint diffuseTex = LoadTexture(diffusePath);
        // Bind and set sampler: glActiveTexture(GL_TEXTURE0);
        //                        glBindTexture(GL_TEXTURE_2D, diffuseTex);
        //                        glUniform1i(locDiffuseMap, 0);
    }
}

/**
 * Example 6: Material Property Interpolation
 */
void Example_MaterialInterpolation()
{
    // Interpolate between two materials
    PhongMaterial mat1, mat2;
    
    // Set up mat1 and mat2...
    
    float blendFactor = 0.5f; // 50% blend
    
    PhongMaterial blended;
    blended.SetKa(glm::mix(mat1.GetKa(), mat2.GetKa(), blendFactor));
    blended.SetKd(glm::mix(mat1.GetKd(), mat2.GetKd(), blendFactor));
    blended.SetKs(glm::mix(mat1.GetKs(), mat2.GetKs(), blendFactor));
    blended.SetNs(glm::mix(mat1.GetNs(), mat2.GetNs(), blendFactor));
}

/**
 * Example 7: Dynamic Material Switching
 */
class RenderObject
{
private:
    std::vector<PhongMaterial> materials;
    int currentMaterialIndex;

public:
    RenderObject() : currentMaterialIndex(0) {}

    void AddMaterial(const PhongMaterial& material)
    {
        materials.push_back(material);
    }

    void SwitchMaterial(int index)
    {
        if (index >= 0 && index < materials.size()) {
            currentMaterialIndex = index;
        }
    }

    PhongMaterial& GetCurrentMaterial()
    {
        return materials[currentMaterialIndex];
    }

    void Render(VisMaterialShaderProg& shader)
    {
        PhongMaterial& mat = GetCurrentMaterial();

        // Set material uniforms
        GLint locKd = glGetUniformLocation(shader.shaderProgId, "Kd");
        glUniform3f(locKd, mat.GetKd().x, mat.GetKd().y, mat.GetKd().z);

        // Render geometry
        // glDrawArrays(...);
    }
};

/**
 * Example 8: Debug Visualization - Normal Inspection
 */
void Example_NormalVisualization()
{
    // Use the normal visualization shader to debug normals
    ShaderProg normalVisShader;
    normalVisShader.LoadFromFiles("shaders/normal_visualize.vs", "shaders/normal_visualize.fs");

    normalVisShader.Bind();

    // Set only MVP matrix (normal visualization doesn't need lighting)
    glm::mat4 mvp = glm::mat4(1.0f);  // Set to actual MVP
    GLint locMVP = normalVisShader.GetLocMVP();
    glUniformMatrix4fv(locMVP, 1, GL_FALSE, glm::value_ptr(mvp));

    // Render geometry
    // glDrawArrays(GL_TRIANGLES, 0, vertexCount);

    normalVisShader.UnBind();

    // Now each pixel shows the normal as RGB color
    // Red = X direction, Green = Y direction, Blue = Z direction
}

/**
 * Example 9: Creating a Custom Material Class
 */
class CustomGlassMaterial : public PhongMaterial
{
public:
    CustomGlassMaterial()
    {
        SetKa(glm::vec3(0.05f, 0.05f, 0.05f));
        SetKd(glm::vec3(0.9f, 0.9f, 0.9f));
        SetKs(glm::vec3(1.0f, 1.0f, 1.0f));
        SetNs(100.0f);
        transparency = 0.8f;
    }

    void SetTransparency(float t)
    {
        transparency = glm::clamp(t, 0.0f, 1.0f);
    }

    float GetTransparency() const
    {
        return transparency;
    }

private:
    float transparency;
};

/**
 * Example 10: Complete Rendering Function
 */
void RenderWithMaterial(
    VisMaterialShaderProg& shader,
    PhongMaterial& material,
    const glm::mat4& model,
    const glm::mat4& view,
    const glm::mat4& projection,
    const glm::vec3& lightPos,
    const glm::vec3& viewPos,
    const glm::vec3& lightColor,
    GLuint vao,
    int vertexCount)
{
    shader.Bind();

    // Calculate matrices
    glm::mat4 mvp = projection * view * model;
    glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));

    // Set transformation matrices
    glUniformMatrix4fv(shader.GetLocMVP(), 1, GL_FALSE, glm::value_ptr(mvp));
    GLint locModel = glGetUniformLocation(shader.shaderProgId, "Model");
    GLint locNormalMatrix = glGetUniformLocation(shader.shaderProgId, "NormalMatrix");
    glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix3fv(locNormalMatrix, 1, GL_FALSE, glm::value_ptr(normalMatrix));

    // Set material properties
    GLint locKa = glGetUniformLocation(shader.shaderProgId, "Ka");
    GLint locKd = glGetUniformLocation(shader.shaderProgId, "Kd");
    GLint locKs = glGetUniformLocation(shader.shaderProgId, "Ks");
    GLint locNs = glGetUniformLocation(shader.shaderProgId, "Ns");

    glUniform3fv(locKa, 1, glm::value_ptr(material.GetKa()));
    glUniform3fv(locKd, 1, glm::value_ptr(material.GetKd()));
    glUniform3fv(locKs, 1, glm::value_ptr(material.GetKs()));
    glUniform1f(locNs, material.GetNs());

    // Set lighting
    GLint locLightPos = glGetUniformLocation(shader.shaderProgId, "LightPos");
    GLint locViewPos = glGetUniformLocation(shader.shaderProgId, "ViewPos");
    GLint locLightColor = glGetUniformLocation(shader.shaderProgId, "LightColor");

    glUniform3fv(locLightPos, 1, glm::value_ptr(lightPos));
    glUniform3fv(locViewPos, 1, glm::value_ptr(viewPos));
    glUniform3fv(locLightColor, 1, glm::value_ptr(lightColor));

    // Render
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    glBindVertexArray(0);

    shader.UnBind();
}

/**
 * Main function demonstrating basic usage
 */
int main()
{
    // Initialize OpenGL context and window here...

    // Load shader program
    VisMaterialShaderProg shader;
    if (!shader.LoadFromFiles("shaders/vis_material.vs", "shaders/vis_material.fs")) {
        // Handle error
        return -1;
    }

    // Create material
    PhongMaterial material;
    material.SetName("Example Material");
    material.SetKd(glm::vec3(0.8f, 0.8f, 0.8f));

    // Setup camera
    glm::mat4 view = glm::lookAt(
        glm::vec3(0.0f, 5.0f, 10.0f),  // Camera position
        glm::vec3(0.0f, 0.0f, 0.0f),   // Look at
        glm::vec3(0.0f, 1.0f, 0.0f)    // Up
    );

    glm::mat4 projection = glm::perspective(
        glm::radians(45.0f),
        800.0f / 600.0f,
        0.1f,
        100.0f
    );

    glm::vec3 lightPos(5.0f, 5.0f, 5.0f);
    glm::vec3 viewPos(0.0f, 5.0f, 10.0f);

    // Render loop
    // while (!window.shouldClose()) {
    //     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //
    //     glm::mat4 model = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime(), glm::vec3(0, 1, 0));
    //
    //     RenderWithMaterial(shader, material, model, view, projection,
    //                        lightPos, viewPos, glm::vec3(1, 1, 1),
    //                        vao, vertexCount);
    //
    //     window.swapBuffers();
    // }

    return 0;
}
