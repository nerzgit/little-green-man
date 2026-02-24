#ifndef GAME_MANAGER_HPP
#define GAME_MANAGER_HPP

#include "../engine/graphics/Renderer.hpp"
#include "Player.hpp"
#include <memory>

class GameManager {
public:
	GameManager(int windowWidth, int windowHeight);

	void update(float deltaTime);
	void render(Renderer& renderer);

private:
	int                     windowWidth_;
	int                     windowHeight_;
	std::unique_ptr<Player> player_;

	void updatePlayer(float deltaTime);
};

#endif // GAME_MANAGER_HPP
