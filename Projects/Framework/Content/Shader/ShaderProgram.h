#pragma once
class ShaderProgram
{
public:
	ShaderProgram(const char* vshPath, const char* fshPath);
	virtual ~ShaderProgram();

	void Use();

	void setBool(const std::string &name, bool value) const
	{
		glUniform1i(glGetUniformLocation(m_Id, name.c_str()), (int)value);
	}
	void setInt(const std::string &name, int value) const
	{
		glUniform1i(glGetUniformLocation(m_Id, name.c_str()), value);
	}
	void setFloat(const std::string &name, float value) const
	{
		glUniform1f(glGetUniformLocation(m_Id, name.c_str()), value);
	}

private:
	unsigned int m_Id;
};