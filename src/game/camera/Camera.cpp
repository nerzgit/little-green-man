#include "Camera.hpp"

#include "../../engine/graphics/Renderer.hpp"
#include "../player/Player.hpp"
#include <algorithm>

Camera::Camera(const Player& player, int windowWidth, int windowHeight,
               float stageWidth, float stageHeight)
    : player_(player)
    , windowWidth_(windowWidth)
    , windowHeight_(windowHeight)
    , stageWidth_(stageWidth)
    , stageHeight_(stageHeight) {
}
void Camera::apply(Renderer& renderer) {
	trackPlayerWithinStage();
	renderer.setCamera(position_, zoom_);
}

// プレイヤーを画面中央に追従しつつ、ステージ外にはみ出さないよう position_
// をクランプする。 ステージがビューより狭い場合は中央に固定する。
void Camera::trackPlayerWithinStage() {
	// zoom_ を考慮したワールド空間でのビューサイズ
	float viewW = static_cast<float>(windowWidth_) * zoom_;
	float viewH = static_cast<float>(windowHeight_) * zoom_;

	// プレイヤーを中央に置いた場合のカメラ左上座標
	position_.x = player_.position.x - viewW / 2.0f;
	position_.y = player_.position.y - viewH / 2.0f;

	// ステージがビューより広ければ境界内にクランプ、狭ければ中央固定
	if (stageWidth_ > viewW) {
		position_.x = std::clamp(position_.x, 0.0f, stageWidth_ - viewW);
	} else {
		position_.x = (stageWidth_ - viewW) / 2.0f;
	}
	if (stageHeight_ > viewH) {
		position_.y = std::clamp(position_.y, 0.0f, stageHeight_ - viewH);
	} else {
		position_.y = (stageHeight_ - viewH) / 2.0f;
	}
}
