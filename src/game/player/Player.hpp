#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "../../engine/core/Entity.hpp"
#include <memory>

class AnimatedSprite;
class Renderer;
class Texture;

class Player : public Entity {
public:
	float speed;

	Player(glm::vec2 position);
	~Player();

	void           update(float deltaTime) override;
	void           draw(Renderer& renderer);
	void           handleInput(float deltaTime);
	const Texture& getSprite() const;

private:
	std::unique_ptr<AnimatedSprite> sprite_;
};

#endif // PLAYER_HPP
