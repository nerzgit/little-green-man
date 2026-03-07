#ifndef DIALOGUE_BOX_HPP
#define DIALOGUE_BOX_HPP

#include <string>

class FontAtlas;
class Renderer;

class DialogueBox {
public:
	DialogueBox(int windowWidth, int windowHeight);

	void open(const std::string& utf8Text);
	void update(float deltaTime);
	void draw(Renderer& renderer, const FontAtlas& font) const;

	bool isVisible() const { return !text_.empty(); }

private:
	int         windowWidth_, windowHeight_;
	std::string text_;
	std::string visibleText_; // update()で更新、draw()はこれを描画するだけ
	float       timer_        = 0.f;
	int         visibleChars_ = 0;
	int         totalChars_   = 0;
};

#endif // DIALOGUE_BOX_HPP
