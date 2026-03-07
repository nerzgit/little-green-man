#include "DialogueBox.hpp"

// glad を GLFW より先に
#include "../graphics/FontAtlas.hpp"
#include "../graphics/Renderer.hpp"
#include "../input/InputManager.hpp"

// clang-format off
#include <GLFW/glfw3.h>
// clang-format on

DialogueBox::DialogueBox(int windowWidth, int windowHeight)
    : windowWidth_(windowWidth), windowHeight_(windowHeight) {
}

void DialogueBox::open(const std::string& utf8Text) {
	text_         = utf8Text;
	visibleText_  = "";
	timer_        = 0.f;
	visibleChars_ = 0;

	// 全文字数を数える（継続バイト 10xxxxxx を除いたバイト数）
	totalChars_ = 0;
	for (unsigned char c : utf8Text)
		if ((c & 0xC0) != 0x80)
			++totalChars_;
}

void DialogueBox::update(float deltaTime) {
	if (text_.empty())
		return;

	// 文字送り
	if (visibleChars_ < totalChars_) {
		timer_ += deltaTime;
		if (timer_ > 0.05f) {
			++visibleChars_;
			timer_ = 0.f;

			// 表示文字列をキャッシュ（draw()はこれを使うだけ）
			const char* p = text_.c_str();
			for (int n = 0; n < visibleChars_ && *p; ++n) {
				++p;
				while (*p && (static_cast<unsigned char>(*p) & 0xC0) == 0x80)
					++p;
			}
			visibleText_ =
			  text_.substr(0, static_cast<size_t>(p - text_.c_str()));
		}
	}

	// Z / Space / Enter で閉じる（全文表示済みのとき）
	if (visibleChars_ >= totalChars_) {
		if (InputManager::isKeyPressed(GLFW_KEY_Z)
		    || InputManager::isKeyPressed(GLFW_KEY_SPACE)
		    || InputManager::isKeyPressed(GLFW_KEY_ENTER)) {
			text_.clear();
		}
	}
}

void DialogueBox::draw(Renderer& renderer, const FontAtlas& font) const {
	if (text_.empty())
		return;

	static constexpr float kMargin  = 20.f;
	static constexpr float kPadding = 14.f;

	const float boxW = static_cast<float>(windowWidth_) - kMargin * 2.f;
	const float boxH = kPadding * 2.f + font.getLineHeight() * 3.f; // 3行分
	const float boxL = kMargin;
	const float boxT = static_cast<float>(windowHeight_) - boxH - kMargin;

	// 会話ボックスの背景
	renderer.drawRectHUD({boxL + boxW / 2.f, boxT + boxH / 2.f}, boxW + 4.f,
	                     boxH + 4.f, {0.5f, 0.75f, 1.0f});

	// 会話ボックスの内側（少し小さくして枠っぽく）
	renderer.drawRectHUD({boxL + boxW / 2.f, boxT + boxH / 2.f}, boxW, boxH,
	                     {0.06f, 0.06f, 0.18f});

	// テキスト（HUD座標で描画）
	renderer.drawTextHUD(
	  font, visibleText_,
	  {boxL + kPadding, boxT + kPadding + font.getFontSize()}, 1.0f,
	  {1.f, 1.f, 1.f});
}
