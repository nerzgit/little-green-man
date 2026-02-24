#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "../engine/core/Entity.hpp"

class Player : public Entity {
public:
	float speed;

	Player(glm::vec2 position);

	void update(float deltaTime) override;
	void handleInput(float deltaTime);
};

#endif // PLAYER_HPP
