#ifndef PARALLAX_OVERLAY_HPP
#define PARALLAX_OVERLAY_HPP

#include "../engine/graphics/Renderer.hpp"
#include "../engine/graphics/Texture.hpp"
#include <memory>
#include <string>

// プレイヤーのX移動に追従してスクロールする全画面テクスチャオーバーレイ
class ParallaxOverlay {
public:
	// プレイヤーに対するスクロール速度比率（0.0=固定、1.0=カメラと同速）
	float speed = 0.3f;

	void load(const std::string& path) {
		texture_ = std::make_unique<Texture>(path);
	}

	void draw(Renderer& renderer, int windowWidth, int windowHeight,
	          float playerX) const {
		if (!texture_)
			return;
		// playerX をUV空間のオフセットに変換
		float uvOffsetX = (playerX * speed) / static_cast<float>(texture_->width);
		renderer.drawParallaxHUD(*texture_, windowWidth, windowHeight, uvOffsetX);
	}

private:
	std::unique_ptr<Texture> texture_;
};

#endif // PARALLAX_OVERLAY_HPP
