#include "LightMirror.hpp"

LightMirror::LightMirror() : direction_(Direction::UPLEFT) {
}

LightMirror::LightMirror(glm::vec2 pos, Direction dir, int tileId)
    : LightTile(pos, tileId), direction_(dir) {
}

LightMirror::~LightMirror() {
}

void LightMirror::startMove(const glm::vec2& from) {
	moveFrom_    = from;
	moveElapsed_ = 0.0f;
}

void LightMirror::update(float deltaTime) {
	if (moveElapsed_ < kMoveDuration)
		moveElapsed_ += deltaTime;
}

glm::vec2 LightMirror::getDrawPosition() const {
	if (moveElapsed_ >= kMoveDuration)
		return position;
	float t = moveElapsed_ / kMoveDuration;
	return moveFrom_ + (position - moveFrom_) * t;
}

Direction LightMirror::redirect(Direction incoming) {
	switch (direction_) {
	case Direction::UPLEFT:
	case Direction::DOWNRIGHT: // ＼型
		switch (incoming) {
		case Direction::RIGHT: return Direction::UP;
		case Direction::DOWN:  return Direction::LEFT;
		case Direction::LEFT:  return Direction::DOWN;
		case Direction::UP:    return Direction::RIGHT;
		default:               return incoming;
		}
	case Direction::UPRIGHT:
	case Direction::DOWNLEFT: // ／型
		switch (incoming) {
		case Direction::LEFT:  return Direction::UP;
		case Direction::DOWN:  return Direction::RIGHT;
		case Direction::RIGHT: return Direction::DOWN;
		case Direction::UP:    return Direction::LEFT;
		default:               return incoming;
		}
	case Direction::UP:
	case Direction::DOWN: // 水平反射
		switch (incoming) {
		case Direction::DOWN: return Direction::UP;
		case Direction::UP:   return Direction::DOWN;
		default:              return incoming;
		}
	case Direction::LEFT:
	case Direction::RIGHT: // 垂直反射
		switch (incoming) {
		case Direction::RIGHT: return Direction::LEFT;
		case Direction::LEFT:  return Direction::RIGHT;
		default:               return incoming;
		}
	}
}
