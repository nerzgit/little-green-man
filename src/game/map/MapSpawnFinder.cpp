#include "MapSpawnFinder.hpp"

#include "../GameConstants.hpp"

namespace MapSpawnFinder {

glm::vec2 spawnPosition(const std::vector<MapLayer>& layers, CharacterType type) {
	if (type == CharacterType::Player) {
		const float ts = GameConstants::kTileSize;
		for (const auto& l : layers) {
			if (l.loader().hasPlayerSpawn()) {
				const float col = static_cast<float>(l.loader().getPlayerSpawnCol());
				const float row = static_cast<float>(l.loader().getPlayerSpawnRow());
				return {col * ts + ts / 2.0f, row * ts + ts / 2.0f};
			}
		}
	}
	return {};
}

std::vector<glm::vec2> spawnPositions(const std::vector<MapLayer>& layers, CharacterType type) {
	if (type == CharacterType::Enemy) {
		const float            ts = GameConstants::kTileSize;
		std::vector<glm::vec2> positions;
		for (const auto& l : layers) {
			for (const auto& [col, row] : l.loader().getEnemySpawns()) {
				positions.emplace_back(col * ts + ts / 2.0f, row * ts + ts / 2.0f);
			}
		}
		return positions;
	}
	return {};
}

} // namespace MapSpawnFinder
