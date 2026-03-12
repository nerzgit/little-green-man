#ifndef LIGHT_MIRROR_HPP
#define LIGHT_MIRROR_HPP

#include "../../engine/core/Entity.hpp"
#include "./Direction.hpp"
#include <unordered_map>

class LightMirror : public Entity {
public:
	LightMirror();
	LightMirror(glm::vec2 pos, Direction dir, int tileId);
	~LightMirror();

	Direction redirect(Direction incoming);
	int       getTileId() const { return tileId_; }

	void      startMove(const glm::vec2& from);
	void      update(float deltaTime);
	glm::vec2 getDrawPosition() const;
	bool      isMoving() const { return moveElapsed_ < kMoveDuration; }

private:
	static constexpr float kMoveDuration = 0.2f;

	Direction direction_;
	int       tileId_;
	glm::vec2 moveFrom_    = {0.0f, 0.0f};
	float     moveElapsed_ = kMoveDuration; // 初期値=完了済み
};

inline const std::unordered_map<int, Direction> kMirrorTileIdToDirection = {
  {1672, Direction::UPLEFT},  {1673, Direction::UP},
  {1674, Direction::UPRIGHT}, {1680, Direction::LEFT},
  {1682, Direction::RIGHT},   {1688, Direction::DOWNLEFT},
  {1689, Direction::DOWN},    {1690, Direction::DOWNRIGHT}};

#endif // LIGHT_MIRROR_HPP