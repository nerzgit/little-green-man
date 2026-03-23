#include "CollisionResolver.hpp"

#include "../../engine/core/Entity.hpp"
#include "../GameConstants.hpp"
#include "../map/Map.hpp"
#include <algorithm>

CollisionResolver::CollisionResolver(Entity& entity, Map& map)
    : entity_(entity), map_(map) {
}

void CollisionResolver::resolve() {
	const float ts          = GameConstants::kTileSize;
	const float r           = entity_.size / 2.0f;
	glm::vec2&  pos         = entity_.position;
	const float stageWidth  = map_.getPixelWidth();
	const float stageHeight = map_.getPixelHeight();

	if (map_.isWallAt(pos.x - r, pos.y)) {
		int col = static_cast<int>((pos.x - r) / ts);
		pos.x   = (col + 1) * ts + r;
	}
	if (map_.isWallAt(pos.x + r, pos.y)) {
		int col = static_cast<int>((pos.x + r) / ts);
		pos.x   = col * ts - r;
	}
	if (map_.isWallAt(pos.x, pos.y - r)) {
		int row = static_cast<int>((pos.y - r) / ts);
		pos.y   = (row + 1) * ts + r;
	}
	if (map_.isWallAt(pos.x, pos.y + r)) {
		int row = static_cast<int>((pos.y + r) / ts);
		pos.y   = row * ts - r;
	}

	pos.x = std::clamp(pos.x, r, stageWidth - r);
	pos.y = std::clamp(pos.y, r, stageHeight - r);
}
