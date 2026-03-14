#include "engine/graphics/Renderer.hpp"
#include "engine/input/InputManager.hpp"
#include "engine/platform/window.hpp"
#include "game/scene/GameScene.hpp"
#include "game/scene/SceneManager.hpp"

#include <iostream>

int main() {
	try {
		const int WINDOW_WIDTH  = 800;
		const int WINDOW_HEIGHT = 600;

		Window window(WINDOW_WIDTH, WINDOW_HEIGHT, "Little Green Man");
		InputManager::init(window.getGLFWWindow());

		Renderer     renderer(WINDOW_WIDTH, WINDOW_HEIGHT);
		SceneManager sceneManager(WINDOW_WIDTH, WINDOW_HEIGHT);

		// 最初のシーン選択
		// TODO:
		//  現在はデバッグようにGameSceneに直接飛ぶようにしているが、タイトル画面を作ったらそちらに切り替える
		sceneManager.switchTo(
		  std::make_unique<GameScene>(WINDOW_WIDTH, WINDOW_HEIGHT));

		// 更新時ループ用コールバック
		window.setUpdateCallback(
		  [&sceneManager](float deltaTime) { sceneManager.update(deltaTime); });

		// 描画時ループ用コールバック
		window.setRenderCallback(
		  [&renderer, &sceneManager]() { sceneManager.render(renderer); });

		window.run();

	} catch (const std::exception &e) {
		std::cerr << "Error: " << e.what() << '\n';
		return 1;
	}
	return 0;
}
