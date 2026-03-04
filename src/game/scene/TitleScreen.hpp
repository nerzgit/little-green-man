#ifndef TITLE_SCREEN_HPP
#define TITLE_SCREEN_HPP

#include "../../engine/graphics/FontAtlas.hpp"
#include "Scene.hpp"
#include <memory>
#include <string>
#include <vector>

// タイトル画面。メニューで「スタート」「おわり」を選択できる。
class TitleScreen : public Scene {
public:
	TitleScreen(int windowWidth, int windowHeight);

	void update(float deltaTime, SceneManager& sm) override;
	void render(Renderer& renderer) override;

private:
	int windowWidth_;
	int windowHeight_;

	std::unique_ptr<FontAtlas> font_;

	std::vector<std::string> menuItems_;
	int                      selectedIndex_ = 0;

	// キー入力の「押した瞬間」を検出するためのフラグ
	bool prevUp_    = false;
	bool prevDown_  = false;
	bool prevEnter_ = false;
	bool prevSpace_ = false;

	void onSelect(SceneManager& sm);
};

#endif // TITLE_SCREEN_HPP
