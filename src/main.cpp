#include "engine/graphics/Renderer.hpp"
#include "engine/input/InputManager.hpp"
#include "engine/platform/window.hpp"
#include "game/scene/SceneManager.hpp"
#include "game/scene/SplashScreen.hpp"

#include <iostream>

int main() {
	try {
		const int WINDOW_WIDTH  = 800;
		const int WINDOW_HEIGHT = 600;

		Window window(WINDOW_WIDTH, WINDOW_HEIGHT, "Little Green Man");
		InputManager::init(window.getGLFWWindow());

		Renderer    renderer(WINDOW_WIDTH, WINDOW_HEIGHT);
		SceneManager sceneManager(WINDOW_WIDTH, WINDOW_HEIGHT);

		// 最初のシーンはスプラッシュ画面
		sceneManager.switchTo(
		  std::make_unique<SplashScreen>(WINDOW_WIDTH, WINDOW_HEIGHT));

		window.setUpdateCallback(
		  [&sceneManager](float deltaTime) { sceneManager.update(deltaTime); });

		window.setRenderCallback(
		  [&renderer, &sceneManager]() { sceneManager.render(renderer); });

		std::cout << "=== Little Green Man ===" << std::endl;
		std::cout << "Controls:" << std::endl;
		std::cout << "  WASD - Move" << std::endl;
		std::cout << "  Mouse Click - Shoot" << std::endl;
		std::cout << "  ESC - Title へ戻る" << std::endl;
		std::cout << "===================" << std::endl;

		window.run();

	} catch (const std::exception &e) {
		std::cerr << "Error: " << e.what() << '\n';
		return 1;
	}
	return 0;
}
