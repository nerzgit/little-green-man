#ifndef ACTION_HPP
#define ACTION_HPP

#include <string>
#include <vector>

struct StoryContext;

// アクションの基底クラス。「何かを実行する方法」だけを知っている。
// ストーリーの条件（いつ起きるか）は一切知らない。
//
// ライフサイクル:
//   start()  → ActionQueue が先頭に来たときに1度だけ呼ばれる
//   update() → isFinished() が true になるまで毎フレーム呼ばれる
class Action {
public:
	virtual ~Action() = default;

	virtual void start(StoryContext& ctx)            = 0;
	virtual void update(float dt, StoryContext& ctx) = 0;
	virtual bool isFinished() const                  = 0;
};

// 複数ページの会話を順番に表示する
class DialogueAction : public Action {
public:
	explicit DialogueAction(std::vector<std::string> lines)
	    : lines_(std::move(lines)) {
	}
	void start(StoryContext& ctx) override;
	void update(float dt, StoryContext& ctx) override;
	bool isFinished() const override { return finished_; }

private:
	std::vector<std::string> lines_;
	int                      pageIndex_ = 0;
	bool                     finished_  = false;
};

// ストーリーフラグを立てる（即座に完了）
class SetFlagAction : public Action {
public:
	explicit SetFlagAction(std::string flag) : flag_(std::move(flag)) {}
	void start(StoryContext& ctx) override;
	void update(float, StoryContext&) override {}
	bool isFinished() const override { return done_; }

private:
	std::string flag_;
	bool        done_ = false;
};

// N 秒間待つ（演出用）
class WaitAction : public Action {
public:
	explicit WaitAction(float seconds) : duration_(seconds) {}
	void start(StoryContext&) override { elapsed_ = 0.f; }
	void update(float dt, StoryContext&) override { elapsed_ += dt; }
	bool isFinished() const override { return elapsed_ >= duration_; }

private:
	float duration_;
	float elapsed_ = 0.f;
};

#endif // ACTION_HPP
