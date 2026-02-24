#include "Renderer.hpp"
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec2 aPos;

uniform mat4 projection;

void main() {
    gl_Position = projection * vec4(aPos, 0.0, 1.0);
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

uniform vec3 color;

void main() {
    FragColor = vec4(color, 1.0);
}
)";

Renderer::Renderer(int windowWidth, int windowHeight)
    : windowWidth_(windowWidth), windowHeight_(windowHeight) {
	initShader();
	initBuffers();
}

Renderer::~Renderer() {
	glDeleteVertexArrays(1, &VAO_);
	glDeleteBuffers(1, &VBO_);
}

void Renderer::initShader() {
	shader_ =
	  std::make_unique<Shader>(vertexShaderSource, fragmentShaderSource);

	// 正投影行列を設定（ピクセル座標系、Y軸下向き）
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(windowWidth_),
	                                  static_cast<float>(windowHeight_), 0.0f);

	shader_->use();
	shader_->setMat4("projection", projection);
}

void Renderer::initBuffers() {
	glGenVertexArrays(1, &VAO_);
	glGenBuffers(1, &VBO_);

	glBindVertexArray(VAO_);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
	                      (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Renderer::clear() {
	glClearColor(0.05f, 0.05f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::drawCircle(const glm::vec2& position, float radius, float r,
                          float g, float b) {
	const int          segments = 20;
	std::vector<float> vertices;

	for (int i = 0; i <= segments; i++) {
		float angle = 2.0f * 3.14159f * i / segments;
		float x     = position.x + radius * std::cos(angle);
		float y     = position.y + radius * std::sin(angle);
		vertices.push_back(x);
		vertices.push_back(y);
	}

	shader_->use();
	shader_->setVec3("color", r, g, b);

	glBindVertexArray(VAO_);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
	             vertices.data(), GL_DYNAMIC_DRAW);

	glDrawArrays(GL_TRIANGLE_FAN, 0, segments + 1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Renderer::drawRect(const glm::vec2& position, float width, float height,
                        float r, float g, float b) {
	float halfWidth  = width / 2.0f;
	float halfHeight = height / 2.0f;

	float vertices[] = {position.x - halfWidth, position.y - halfHeight,
	                    position.x + halfWidth, position.y - halfHeight,
	                    position.x + halfWidth, position.y + halfHeight,
	                    position.x - halfWidth, position.y + halfHeight};

	shader_->use();
	shader_->setVec3("color", r, g, b);

	glBindVertexArray(VAO_);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
