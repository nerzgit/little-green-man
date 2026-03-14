#include "LightBeam.hpp"
#include "../../engine/graphics/Renderer.hpp"
#include "../GameConstants.hpp"
#include <cmath>

LightBeam::LightBeam() {
}

LightBeam::~LightBeam() {
}

void LightBeam::clear() {
	waypoints_.clear();
}

void LightBeam::draw(Renderer& renderer) const {
	// セル1個分の移動量（thickness px ずつ並べる）
	const float step = thickness;

	for (size_t i = 0; i + 1 < waypoints_.size(); ++i) {
		glm::vec2 from = waypoints_[i].position;
		// from → to へのベクトル
		glm::vec2 diff = waypoints_[i + 1].position - from;
		// 水平 or 垂直の移動距離（軸方向の絶対値が大きい方）
		float dist = std::max(std::abs(diff.x), std::abs(diff.y));
		// 同座標のウェイポイント（ミラー折れ曲がり点）はスキップ
		if (dist == 0.0f)
			continue;
		// 進行方向の単位ベクトル
		glm::vec2 dir = diff / dist;
		// 距離をステップで割った個数（両端を含めるため +1）
		int count = static_cast<int>(dist / step) + 1;

		for (int k = 0; k < count; ++k) {
			glm::vec2 pos = from + dir * (step * k);
			// 外側グロー（大きく薄い）
			renderer.drawRectGlow(pos, thickness * 4.0f, thickness * 4.0f,
			                      {1.0f, 0.9f, 0.3f}, 0.01f);
			// 内側グロー
			renderer.drawRectGlow(pos, thickness * 2.0f, thickness * 2.0f,
			                      {1.0f, 1.0f, 0.5f}, 0.06f);
			// コア（明るい白）
			renderer.drawRectGlow(pos, thickness * 0.5f, thickness * 0.5f,
			                      {1.0f, 1.0f, 0.9f}, 0.9f);
		}
	}
}

void LightBeam::addWaypoint(const glm::vec2& position, Direction outDir) {
	constexpr float kTileHalf = GameConstants::kTilePixels / 2.0f;
	glm::vec2 center = {position.x + kTileHalf, position.y + kTileHalf};
	waypoints_.push_back({center, outDir});
}