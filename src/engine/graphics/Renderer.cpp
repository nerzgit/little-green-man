#include "Renderer.hpp"
#include <glm/gtc/matrix_transform.hpp>

const char* textureVertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 projection;

void main() {
    gl_Position = projection * vec4(aPos, 0.0, 1.0);
    TexCoord = aTexCoord;
}
)";

const char* textureFragmentShaderSource = R"(
#version 330 core
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D tex;

void main() {
    FragColor = texture(tex, TexCoord);
}
)";

Renderer::Renderer(int windowWidth, int windowHeight)
    : windowWidth_(windowWidth), windowHeight_(windowHeight) {
	initTextureShader();
	initTextureBuffers();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

Renderer::~Renderer() {
	glDeleteVertexArrays(1, &textureVAO_);
	glDeleteBuffers(1, &textureVBO_);
}

void Renderer::clear() {
	glClearColor(0.05f, 0.05f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::initTextureShader() {
	textureShader_ = std::make_unique<Shader>(textureVertexShaderSource,
	                                          textureFragmentShaderSource);

	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(windowWidth_),
	                                  static_cast<float>(windowHeight_), 0.0f);

	textureShader_->use();
	textureShader_->setMat4("projection", projection);
}

void Renderer::initTextureBuffers() {
	glGenVertexArrays(1, &textureVAO_);
	glGenBuffers(1, &textureVBO_);

	glBindVertexArray(textureVAO_);
	glBindBuffer(GL_ARRAY_BUFFER, textureVBO_);

	// position: location=0, 2 floats
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
	                      (void*)0);
	glEnableVertexAttribArray(0);

	// texcoord: location=1, 2 floats
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
	                      (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Renderer::drawSprite(const Texture& texture, const glm::vec2& position,
                          float width, float height) {
	float halfWidth  = width / 2.0f;
	float halfHeight = height / 2.0f;

	// x, y, u, v per vertex (center-based position, Y-down)
	float vertices[] = {
	  position.x - halfWidth, position.y - halfHeight, 0.0f, 0.0f,
	  position.x + halfWidth, position.y - halfHeight, 1.0f, 0.0f,
	  position.x + halfWidth, position.y + halfHeight, 1.0f, 1.0f,
	  position.x - halfWidth, position.y + halfHeight, 0.0f, 1.0f,
	};

	textureShader_->use();
	textureShader_->setInt("tex", 0);
	texture.bind(0);

	glBindVertexArray(textureVAO_);
	glBindBuffer(GL_ARRAY_BUFFER, textureVBO_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
