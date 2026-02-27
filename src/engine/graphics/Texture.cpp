#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Texture.hpp"
#include <iostream>

Texture::Texture(const std::string& path) : id(0), width(0), height(0) {
	stbi_set_flip_vertically_on_load(true);

	int            channels;
	unsigned char* data =
	  stbi_load(path.c_str(), &width, &height, &channels, 0);

	if (!data) {
		std::cerr << "Failed to load texture: " << path << "\n";
		return;
	}

	GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
	             GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(int width, int height, const unsigned char* rgba)
    : id(0), width(width), height(height) {
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
	             GL_UNSIGNED_BYTE, rgba);

	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture() {
	if (id != 0) {
		glDeleteTextures(1, &id);
	}
}

void Texture::bind(GLuint unit) const {
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, id);
}
