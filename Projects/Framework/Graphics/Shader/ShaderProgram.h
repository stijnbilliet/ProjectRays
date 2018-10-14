#pragma once
class ShaderProgram
{
public:
	ShaderProgram(const char* vshPath, const char* fshPath);
	virtual ~ShaderProgram();

	void Use();
	unsigned int GetId() const;

private:
	unsigned int m_Id;
};