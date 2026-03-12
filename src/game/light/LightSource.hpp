#ifndef LIGHT_SOURCE_HPP
#define LIGHT_SOURCE_HPP

#include "../../engine/core/Entity.hpp"
#include "./Direction.hpp"
#include <unordered_map>

class LightSource : public Entity {
public:
	LightSource();
	LightSource(glm::vec2 pos, Direction dir, int tileId);
	~LightSource();
	Direction getDirection() const;
	int       getTileId() const { return tileId_; }

private:
	Direction direction_;
	int       tileId_;
};

// TODO: 光源の向きバリアントが増えたらIDと方向を追加する
inline const std::unordered_map<int, Direction> kSourceTileIdToDirection = {
  {1681, Direction::DOWN},
};

#endif // LIGHT_SOURCE_HPP