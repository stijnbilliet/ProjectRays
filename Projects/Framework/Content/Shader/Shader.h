#pragma once
class Shader
{
public:
	Shader(const char* shaderPath, GLint shaderType);
	virtual ~Shader();

	unsigned int GetShaderId() const;
private:
	unsigned int m_Id;
};