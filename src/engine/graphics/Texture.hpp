#ifndef TEXTURE_HPP
#define TEXTURE_HPP

// clang-format off
#include <glad/glad.h>
// clang-format on

#include <string>

class Texture {
public:
	GLuint id;
	int    width;
	int    height;

	Texture(const std::string& path);
	Texture(int width, int height, const unsigned char* rgba);
	~Texture();

	void bind(GLuint unit = 0) const;
};

#endif // TEXTURE_HPP
