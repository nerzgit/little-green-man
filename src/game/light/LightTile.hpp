#ifndef LIGHT_TILE_HPP
#define LIGHT_TILE_HPP

#include "../../engine/core/Entity.hpp"

class LightTile : public Entity {
public:
	LightTile() : Entity(), tileId_(-1) {
	}
	LightTile(glm::vec2 pos, int tileId) : Entity(pos, 10.0f), tileId_(tileId) {
	}

	int               getTileId() const { return tileId_; }
	virtual glm::vec2 getDrawPosition() const { return position; }

protected:
	int tileId_;
};

#endif // LIGHT_TILE_HPP
