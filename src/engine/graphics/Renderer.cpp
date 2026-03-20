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
uniform vec4 uColor;

void main() {
    FragColor = texture(tex, TexCoord) * uColor;
}
)";

Renderer::Renderer(int windowWidth, int windowHeight)
    : windowWidth_(windowWidth), windowHeight_(windowHeight) {
	initTextureShader();
	initTextureBuffers();

	const unsigned char white[4] = {255, 255, 255, 255};
	whiteTexture_                = std::make_unique<Texture>(1, 1, white);

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

	worldProjection_ = glm::ortho(0.0f, static_cast<float>(windowWidth_),
	                              static_cast<float>(windowHeight_), 0.0f);

	textureShader_->use();
	textureShader_->setMat4("projection", worldProjection_);
	textureShader_->setVec4("uColor", 1.0f, 1.0f, 1.0f, 1.0f);
}

void Renderer::setCamera(const glm::vec2& cameraPos, float zoom) {
	float viewW = static_cast<float>(windowWidth_) * zoom;
	float viewH = static_cast<float>(windowHeight_) * zoom;

	worldProjection_ = glm::ortho(cameraPos.x, cameraPos.x + viewW,
	                              cameraPos.y + viewH, cameraPos.y);

	textureShader_->use();
	textureShader_->setMat4("projection", worldProjection_);
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
