#include "LightSource.hpp"

LightSource::LightSource() : direction_(Direction::RIGHT) {
}

LightSource::LightSource(glm::vec2 pos, Direction dir, int tileId)
    : LightTile(pos, tileId), direction_(dir) {
}

LightSource::~LightSource() {
}

Direction LightSource::getDirection() const {
	return direction_;
}
