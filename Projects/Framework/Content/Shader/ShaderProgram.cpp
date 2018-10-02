#include "FrameworkPCH.h"
#include "ShaderProgram.h"
#include "Shader.h"

ShaderProgram::ShaderProgram(const char * vshPath, const char * fshPath)
{
	Shader vertexShader(vshPath, GL_VERTEX_SHADER);
	Shader fragmentShader(fshPath, GL_FRAGMENT_SHADER);

	int success;
	char infoLog[512];

	m_Id = glCreateProgram();
	glAttachShader(m_Id, vertexShader.GetShaderId());
	glAttachShader(m_Id, fragmentShader.GetShaderId());
	glLinkProgram(m_Id);
	glGetProgramiv(m_Id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_Id, 512, NULL, infoLog);
		std::cout << "ERROR::SHADERPROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader.GetShaderId());
	glDeleteShader(fragmentShader.GetShaderId());
}

ShaderProgram::~ShaderProgram()
{
}

void ShaderProgram::Use()
{
	glUseProgram(m_Id);
}