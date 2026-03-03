#include "CollisionResolver.hpp"

#include "../map/Map.hpp"
#include "../map/MapLoader.hpp"
#include "../player/Player.hpp"
#include <algorithm>

CollisionResolver::CollisionResolver(Player& player, Map& map, float stageWidth,
                                     float stageHeight)
    : player_(player), map_(map), stageWidth_(stageWidth),
      stageHeight_(stageHeight) {}

void CollisionResolver::resolve() {
	const float ts  = MapLoader::kTileSize;
	const float r   = player_.size;
	glm::vec2&  pos = player_.position;

	// X軸：左端・右端が壁に入ったら押し戻す
	if (map_.isWallAt(pos.x - r, pos.y)) {
		int col = static_cast<int>((pos.x - r) / ts);
		pos.x   = (col + 1) * ts + r;
	}
	if (map_.isWallAt(pos.x + r, pos.y)) {
		int col = static_cast<int>((pos.x + r) / ts);
		pos.x   = col * ts - r;
	}

	// Y軸：上端・下端が壁に入ったら押し戻す
	if (map_.isWallAt(pos.x, pos.y - r)) {
		int row = static_cast<int>((pos.y - r) / ts);
		pos.y   = (row + 1) * ts + r;
	}
	if (map_.isWallAt(pos.x, pos.y + r)) {
		int row = static_cast<int>((pos.y + r) / ts);
		pos.y   = row * ts - r;
	}

	pos.x = std::clamp(pos.x, r, stageWidth_ - r);
	pos.y = std::clamp(pos.y, r, stageHeight_ - r);
}
