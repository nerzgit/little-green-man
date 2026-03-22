#ifndef PARALLAX_OVERLAY_HPP
#define PARALLAX_OVERLAY_HPP

#include "../../engine/graphics/Renderer.hpp"
#include "../../engine/graphics/Texture.hpp"
#include <memory>
#include <string>

// プレイヤーのX移動に追従してスクロールする全画面テクスチャオーバーレイ
class ParallaxOverlay {
public:
	// プレイヤーに対するスクロール速度比率（0.0=固定、1.0=カメラと同速）
	float speed = 0.3f;
	// playerY が増えるほど幅が縮む係数。width = texW * max(0, 1 - playerY *
	// yNarrowFactor)
	float yNarrowFactor = 0.0003f;

	void load(const std::string& path) {
		texture_ = std::make_unique<Texture>(path);
	}

	void draw(Renderer& renderer, int windowWidth, int windowHeight,
	          float playerX, float playerY) const {
		if (!texture_)
			return;
		float uvOffsetX =
		  (playerX * speed) / static_cast<float>(texture_->width);
		float widthScale = std::max(0.0f, 1.0f - playerY * yNarrowFactor);
		int   drawWidth  = static_cast<int>(texture_->width * widthScale);
		renderer.drawParallaxHUD(*texture_, drawWidth, windowHeight, uvOffsetX);
	}

private:
	std::unique_ptr<Texture> texture_;
};

#endif // PARALLAX_OVERLAY_HPP
