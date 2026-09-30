#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include "headers.h"

// ShaderProg Declarations.
class ShaderProg
{
public:
	// ShaderProg Public Methods.
	ShaderProg();
	~ShaderProg();

	bool LoadFromFiles(const std::string vsFilePath, const std::string fsFilePath);
	void Bind() { glUseProgram(shaderProgId); };
	void UnBind() { glUseProgram(0); };

	GLint GetLocMVP() const { return locMVP; }

protected:
	// ShaderProg Protected Methods.
	virtual void GetUniformVariableLocation();

	// ShaderProg Protected Data.
	GLuint shaderProgId;

private:
	// ShaderProg Private Methods.
	GLuint AddShader(const std::string& sourceText, GLenum shaderType);
	static bool LoadShaderTextFromFile(const std::string filePath, std::string& sourceText);

	// ShaderProg Private Data.
	GLint locMVP;
};

// ------------------------------------------------------------------------------------------------

// VisMaterialShaderProg Declarations.
class VisMaterialShaderProg : public ShaderProg
{
public:
	// VisMaterialShaderProg Public Methods.
	VisMaterialShaderProg();
	~VisMaterialShaderProg();

	GLint GetLocKd() const { return locKd; }

protected:
	// VisMaterialShaderProg Protected Methods.
	void GetUniformVariableLocation() override;

private:
	// VisMaterialShaderProg Private Data.
	GLint locKd;
};

#endif
