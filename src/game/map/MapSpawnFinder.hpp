#ifndef MAP_SPAWN_FINDER_HPP
#define MAP_SPAWN_FINDER_HPP

#include "../CharacterType.hpp"
#include "MapLayer.hpp"
#include <glm/glm.hpp>
#include <vector>

namespace MapSpawnFinder {
	glm::vec2              spawnPosition(const std::vector<MapLayer>& layers, CharacterType type);
	std::vector<glm::vec2> spawnPositions(const std::vector<MapLayer>& layers, CharacterType type);
}

#endif // MAP_SPAWN_FINDER_HPP
