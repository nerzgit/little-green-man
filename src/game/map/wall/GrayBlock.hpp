#ifndef GRAY_BLOCK_HPP
#define GRAY_BLOCK_HPP

#include "../../../engine/graphics/Texture.hpp"
#include <glm/glm.hpp>

class Renderer;

class GrayBlock {
public:
	GrayBlock();

	void draw(Renderer& renderer, glm::vec2 position) const;

private:
	Texture texture_;
};

#endif // GRAY_BLOCK_HPP
