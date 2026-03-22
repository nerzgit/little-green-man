#ifndef SPEECH_BUBBLE_HPP
#define SPEECH_BUBBLE_HPP

#include <memory>
#include <queue>
#include <string>

class DialogueBox;
class Renderer;

// フォント・DialogueBox・メッセージキューをまとめた静的会話UIクラス。
// インスタンスは生成せず、すべて静的メソッド経由で操作する。
class SpeechBubble {
public:
	SpeechBubble() = delete;

	// GameScene の初期化時に1度だけ呼ぶ
	static void init(int windowWidth, int windowHeight);

	// メッセージをキューに追加する
	static void enqueue(const std::string& utf8Text);

	// 毎フレーム GameScene から呼ぶ
	static void update(float deltaTime);
	static void draw(Renderer& renderer);

	// キューに残りがある、または表示中のメッセージがある場合に true
	// ゲームの一時停止判定に使う
	static bool isBlocking();

private:
	static std::unique_ptr<DialogueBox> dialogueBox_;
	static std::queue<std::string>      messageQueue_;
};

#endif // SPEECH_BUBBLE_HPP
