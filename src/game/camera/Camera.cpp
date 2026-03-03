#include "Camera.hpp"

#include "../../engine/graphics/Renderer.hpp"
#include "../player/Player.hpp"
#include <algorithm>

Camera::Camera(const Player& player, int windowWidth, int windowHeight,
               float stageWidth, float stageHeight)
    : player_(player), windowWidth_(windowWidth), windowHeight_(windowHeight),
      stageWidth_(stageWidth), stageHeight_(stageHeight) {}

void Camera::apply(Renderer& renderer) {
	constexpr float zoom  = 2.0f;
	float           viewW = static_cast<float>(windowWidth_) * zoom;
	float           viewH = static_cast<float>(windowHeight_) * zoom;

	position_.x = player_.position.x - viewW / 2.0f;
	position_.y = player_.position.y - viewH / 2.0f;
	position_.x = std::clamp(position_.x, 0.0f, stageWidth_ - viewW);
	position_.y = std::clamp(position_.y, 0.0f, stageHeight_ - viewH);

	renderer.setCamera(position_);
}
