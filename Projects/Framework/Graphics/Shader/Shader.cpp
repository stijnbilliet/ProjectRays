#include "FrameworkPCH.h"
#include "Shader.h"

Shader::Shader(const char* shaderPath, GLint shaderType)
{
	// 1. retrieve source code from shaderPath
	std::string shaderSource{};
	std::ifstream shaderFile{};

	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		shaderFile.open(shaderPath);

		std::stringstream shaderStream;
		// read file buffer content into streams
		shaderStream << shaderFile.rdbuf();
		// close file handle
		shaderFile.close();
		// convert stream into string
		shaderSource = shaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	//Store shader in char array
	const char* shaderSourceData = shaderSource.c_str();

	// 2. compile shader
	unsigned int shader; //openGL id
	int success;
	char infoLog[512]; //store debug info

	shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderSourceData, NULL);
	glCompileShader(shader);

	// print compile errors if any
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	//store shader id
	m_Id = shader;
}

Shader::~Shader()
{
}

unsigned int Shader::GetShaderId() const
{
	return m_Id;
}
