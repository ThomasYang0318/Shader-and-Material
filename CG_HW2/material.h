#ifndef MATERIAL_H
#define MATERIAL_H

#include "headers.h"
#include "shaderprog.h"

// Material Declarations.
class Material
{
public:
	// Material Public Methods.
	Material() { name = "Default"; };
	virtual ~Material() {};

	void SetName(const std::string mtlName) { name = mtlName; }
	std::string GetName() const { return name; }

protected:	
	// Material Protected Data.
	std::string name;
};

// ------------------------------------------------------------------------------------------------

// PhongMaterial Declarations.
class PhongMaterial : public Material
{
public:
	// PhongMaterial Public Methods.
	PhongMaterial() {
		Ka = glm::vec3(0.2f, 0.2f, 0.2f);
		Kd = glm::vec3(0.8f, 0.8f, 0.8f);
		Ks = glm::vec3(1.0f, 1.0f, 1.0f);
		Ns = 32.0f;
		name = "Phong Material";
	};
	~PhongMaterial() {};

	void SetKa(const glm::vec3 ka) { Ka = ka; }
	void SetKd(const glm::vec3 kd) { Kd = kd; }
	void SetKs(const glm::vec3 ks) { Ks = ks; }
	void SetNs(const float n) { Ns = n; }

	const glm::vec3 GetKa() const { return Ka; }
	const glm::vec3 GetKd() const { return Kd; }
	const glm::vec3 GetKs() const { return Ks; }
	const float GetNs() const { return Ns; }

private:
	// PhongMaterial Private Data.
	glm::vec3 Ka;
	glm::vec3 Kd;
	glm::vec3 Ks;
	float Ns;
};

// ------------------------------------------------------------------------------------------------

// MetallicMaterial Declarations (PBR-like).
class MetallicMaterial : public Material
{
public:
	// MetallicMaterial Public Methods.
	MetallicMaterial() {
		Kd = glm::vec3(0.5f, 0.5f, 0.5f);
		metallic = 1.0f;
		roughness = 0.2f;
		name = "Metallic Material";
	};
	~MetallicMaterial() {};

	void SetKd(const glm::vec3 kd) { Kd = kd; }
	void SetMetallic(const float m) { metallic = glm::clamp(m, 0.0f, 1.0f); }
	void SetRoughness(const float r) { roughness = glm::clamp(r, 0.0f, 1.0f); }

	const glm::vec3 GetKd() const { return Kd; }
	const float GetMetallic() const { return metallic; }
	const float GetRoughness() const { return roughness; }

private:
	// MetallicMaterial Private Data.
	glm::vec3 Kd;
	float metallic;
	float roughness;
};

// ------------------------------------------------------------------------------------------------

// DielectricMaterial Declarations (for insulators like plastic, glass, etc).
class DielectricMaterial : public Material
{
public:
	// DielectricMaterial Public Methods.
	DielectricMaterial() {
		Kd = glm::vec3(0.9f, 0.9f, 0.9f);
		Ks = glm::vec3(0.5f, 0.5f, 0.5f);
		refractiveIndex = 1.5f;
		roughness = 0.1f;
		name = "Dielectric Material";
	};
	~DielectricMaterial() {};

	void SetKd(const glm::vec3 kd) { Kd = kd; }
	void SetKs(const glm::vec3 ks) { Ks = ks; }
	void SetRefractiveIndex(const float ior) { refractiveIndex = ior; }
	void SetRoughness(const float r) { roughness = glm::clamp(r, 0.0f, 1.0f); }

	const glm::vec3 GetKd() const { return Kd; }
	const glm::vec3 GetKs() const { return Ks; }
	const float GetRefractiveIndex() const { return refractiveIndex; }
	const float GetRoughness() const { return roughness; }

private:
	// DielectricMaterial Private Data.
	glm::vec3 Kd;
	glm::vec3 Ks;
	float refractiveIndex;
	float roughness;
};

// ------------------------------------------------------------------------------------------------

// TexturedMaterial Declarations.
class TexturedMaterial : public PhongMaterial
{
public:
	// TexturedMaterial Public Methods.
	TexturedMaterial() : PhongMaterial() {
		diffuseTexturePath = "";
		normalMapPath = "";
		specularTexturePath = "";
		name = "Textured Material";
	};
	~TexturedMaterial() {};

	void SetDiffuseTexture(const std::string path) { diffuseTexturePath = path; }
	void SetNormalMap(const std::string path) { normalMapPath = path; }
	void SetSpecularTexture(const std::string path) { specularTexturePath = path; }

	const std::string GetDiffuseTexture() const { return diffuseTexturePath; }
	const std::string GetNormalMap() const { return normalMapPath; }
	const std::string GetSpecularTexture() const { return specularTexturePath; }

private:
	// TexturedMaterial Private Data.
	std::string diffuseTexturePath;
	std::string normalMapPath;
	std::string specularTexturePath;
};

#endif

