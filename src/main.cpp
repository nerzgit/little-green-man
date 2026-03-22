#include "engine/graphics/Renderer.hpp"
#include "engine/input/InputManager.hpp"
#include "engine/platform/window.hpp"
#include "game/GameConstants.hpp"
#include "game/scene/GameScene.hpp"
#include "game/scene/SceneManager.hpp"

#include <iostream>

int main() {
	try {
		const int WINDOW_WIDTH  = 800;
		const int WINDOW_HEIGHT = 600;

		Window window(WINDOW_WIDTH, WINDOW_HEIGHT, GameConstants::kGameTitle);
		InputManager::init(window.getGLFWWindow());

		Renderer renderer(WINDOW_WIDTH, WINDOW_HEIGHT);

		// TODO: タイトル画面を作ったら GameScene から切り替える
		SceneManager sceneManager(
		  WINDOW_WIDTH,
		  WINDOW_HEIGHT,
		  std::make_unique<GameScene>(WINDOW_WIDTH, WINDOW_HEIGHT));

		window.run([&](float deltaTime) { sceneManager.update(deltaTime); },
		           [&]() { sceneManager.render(renderer); });

	} catch (const std::exception &e) {
		std::cerr << "Error: " << e.what() << '\n';
		return 1;
	}
	return 0;
}
