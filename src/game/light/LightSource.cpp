#include "LightSource.hpp"

LightSource::LightSource() : direction_(Direction::RIGHT), tileId_(-1) {
}

LightSource::LightSource(glm::vec2 pos, Direction dir, int tileId)
    : Entity(pos, 10.0f), direction_(dir), tileId_(tileId) {
}

LightSource::~LightSource() {
}

Direction LightSource::getDirection() const {
	return direction_;
}
