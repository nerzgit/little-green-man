#ifndef GAME_MANAGER_HPP
#define GAME_MANAGER_HPP

#include <glm/glm.hpp>
#include <memory>

class Player;
class Renderer;

class GameManager {
public:
	GameManager(int windowWidth, int windowHeight);
	~GameManager();

	void update(float deltaTime);
	void render(Renderer& renderer);

private:
	static constexpr float kStageWidth  = 1600.0f;
	static constexpr float kStageHeight = 1200.0f;

	int                     windowWidth_;
	int                     windowHeight_;
	glm::vec2               cameraPos_{0.0f, 0.0f};
	std::unique_ptr<Player> player_;

	void updatePlayer(float deltaTime);
	void updateCamera(Renderer& renderer);
};

#endif // GAME_MANAGER_HPP
