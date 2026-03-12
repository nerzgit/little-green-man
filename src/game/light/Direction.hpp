#ifndef DIRECTION_HPP
#define DIRECTION_HPP

#include <glm/glm.hpp>

enum class Direction {
	UP,
	DOWN,
	LEFT,
	RIGHT,
	UPLEFT,
	UPRIGHT,
	DOWNLEFT,
	DOWNRIGHT
};

inline void turnDirection(Direction& dir) {
	switch (dir) {
	case Direction::UP: dir = Direction::UPRIGHT; break;
	case Direction::DOWN: dir = Direction::DOWNRIGHT; break;
	case Direction::LEFT: dir = Direction::UPLEFT; break;
	case Direction::RIGHT: dir = Direction::DOWNRIGHT; break;
	case Direction::UPLEFT: dir = Direction::UP; break;
	case Direction::UPRIGHT: dir = Direction::RIGHT; break;
	case Direction::DOWNLEFT: dir = Direction::LEFT; break;
	case Direction::DOWNRIGHT: dir = Direction::DOWN; break;
	}
}

inline glm::vec2 nextPosition(glm::vec2 pos, Direction dir, float tileSize) {
	int dx = 0;
	int dy = 0;

	switch (dir) {
	case Direction::UP: dy = -1; break;
	case Direction::UPRIGHT:
		dx = 1;
		dy = -1;
		break;
	case Direction::RIGHT: dx = 1; break;
	case Direction::DOWNRIGHT:
		dx = 1;
		dy = 1;
		break;
	case Direction::DOWN: dy = 1; break;
	case Direction::DOWNLEFT:
		dx = -1;
		dy = 1;
		break;
	case Direction::LEFT: dx = -1; break;
	case Direction::UPLEFT:
		dx = -1;
		dy = -1;
		break;
	}

	return {pos.x + dx * tileSize, pos.y + dy * tileSize};
}

#endif // DIRECTION_HPP