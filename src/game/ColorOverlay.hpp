#ifndef COLOR_OVERLAY_HPP
#define COLOR_OVERLAY_HPP

#include "../engine/graphics/Renderer.hpp"
#include <glm/glm.hpp>

// ゲーム全体の色味を変えるための全画面オーバーレイ
class ColorOverlay {
public:
	glm::vec4 color = {0.0f, 0.0f, 0.0f, 0.0f}; // rgba（alpha=0で非表示）

	void draw(Renderer& renderer, int windowWidth, int windowHeight) const {
		if (color.a <= 0.0f)
			return;
		glm::vec2 center = {windowWidth / 2.0f, windowHeight / 2.0f};
		renderer.drawRectHUD(center, static_cast<float>(windowWidth),
		                     static_cast<float>(windowHeight),
		                     {color.r, color.g, color.b}, color.a);
	}
};

#endif // COLOR_OVERLAY_HPP
