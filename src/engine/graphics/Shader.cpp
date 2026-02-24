#include "Shader.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

Shader::Shader(const char* vertexSource, const char* fragmentSource) {
	GLuint vertexShader   = compileShader(GL_VERTEX_SHADER, vertexSource);
	GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);

	programID = glCreateProgram();
	glAttachShader(programID, vertexShader);
	glAttachShader(programID, fragmentShader);
	glLinkProgram(programID);

	GLint success;
	glGetProgramiv(programID, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(programID, 512, nullptr, infoLog);
		std::cerr << "Shader program linking failed:\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

Shader::~Shader() {
	glDeleteProgram(programID);
}

void Shader::use() const {
	glUseProgram(programID);
}

void Shader::setVec3(const std::string& name, float x, float y, float z) const {
	GLint location = glGetUniformLocation(programID, name.c_str());
	glUniform3f(location, x, y, z);
}

void Shader::setMat4(const std::string& name, const glm::mat4& value) const {
	GLint location = glGetUniformLocation(programID, name.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

GLuint Shader::compileShader(GLenum type, const char* source) {
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader);

	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);
		std::cerr << "Shader compilation failed:\n" << infoLog << std::endl;
	}

	return shader;
}
