#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>

class Player;
class Renderer;

class Camera {
public:
	Camera(const Player& player, int windowWidth, int windowHeight,
	       float stageWidth, float stageHeight);

	void apply(Renderer& renderer);

private:
	const Player& player_;
	int           windowWidth_;
	int           windowHeight_;
	float         stageWidth_;
	float         stageHeight_;
	glm::vec2     position_{0.0f, 0.0f};
};

#endif // CAMERA_HPP
