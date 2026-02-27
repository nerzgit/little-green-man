#ifndef SHADER_HPP
#define SHADER_HPP

// clang-format off
#include <glad/glad.h>
// clang-format on

#include <glm/glm.hpp>
#include <string>

class Shader {
public:
	GLuint programID;

	Shader(const char* vertexSource, const char* fragmentSource);
	~Shader();

	void use() const;
	void setInt(const std::string& name, int value) const;
	void setVec3(const std::string& name, float x, float y, float z) const;
	void setMat4(const std::string& name, const glm::mat4& value) const;

private:
	GLuint compileShader(GLenum type, const char* source);
};

#endif // SHADER_HPP
