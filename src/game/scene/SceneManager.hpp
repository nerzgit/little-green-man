#ifndef SCENE_MANAGER_HPP
#define SCENE_MANAGER_HPP

#include "Scene.hpp"
#include <memory>

class Renderer;

class SceneManager {
public:
	SceneManager(int windowWidth, int windowHeight);

	// 次のフレームで遷移するシーンを予約する
	void switchTo(std::unique_ptr<Scene> scene);

	void update(float deltaTime);
	void render(Renderer& renderer);

	int getWindowWidth() const { return windowWidth_; }
	int getWindowHeight() const { return windowHeight_; }

private:
	int windowWidth_;
	int windowHeight_;

	std::unique_ptr<Scene> current_;
	std::unique_ptr<Scene> next_;  // 予約済みの次シーン
};

#endif // SCENE_MANAGER_HPP
