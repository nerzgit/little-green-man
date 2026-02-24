#include "engine/graphics/Renderer.hpp"
#include "engine/input/InputManager.hpp"
#include "engine/platform/window.hpp"
#include "game/GameManager.hpp"

#include <iostream>

int main() {
	try {
		const int WINDOW_WIDTH  = 800;
		const int WINDOW_HEIGHT = 600;

		Window window(WINDOW_WIDTH, WINDOW_HEIGHT, "Little Green Man");
		InputManager::init(window.getGLFWWindow());

		Renderer    renderer(WINDOW_WIDTH, WINDOW_HEIGHT);
		GameManager gameManager(WINDOW_WIDTH, WINDOW_HEIGHT);

		// 更新コールバックを設定
		window.setUpdateCallback(
		  [&gameManager](float deltaTime) { gameManager.update(deltaTime); });

		// 描画コールバックを設定
		window.setRenderCallback(
		  [&renderer, &gameManager]() { gameManager.render(renderer); });

		std::cout << "=== Little Green Man ===" << std::endl;
		std::cout << "Controls:" << std::endl;
		std::cout << "  WASD - Move" << std::endl;
		std::cout << "  Mouse Click - Shoot" << std::endl;
		std::cout << "  ESC - Exit" << std::endl;
		std::cout << "===================" << std::endl;

		window.run();

	} catch (const std::exception &e) {
		std::cerr << "Error: " << e.what() << '\n';
		return 1;
	}
	return 0;
}
