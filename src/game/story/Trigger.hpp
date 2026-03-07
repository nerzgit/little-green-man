#ifndef TRIGGER_HPP
#define TRIGGER_HPP

struct StoryContext;

// トリガーの基底クラス。条件を「検出する方法」だけを知っている。
// ストーリーの内容（何を起こすか）は一切知らない。
class Trigger {
public:
	virtual ~Trigger() = default;

	// 条件が満たされているか判定する。副作用を持たせてよい（MapLoadTrigger 等）。
	virtual bool check(const StoryContext& ctx) const = 0;
};

// プレイヤーがワールド座標の矩形に入ったら発火
class ZoneTrigger : public Trigger {
public:
	ZoneTrigger(float x, float y, float w, float h)
	    : x_(x), y_(y), w_(w), h_(h) {
	}
	bool check(const StoryContext& ctx) const override;

private:
	float x_, y_, w_, h_;
};

// マップがロードされた瞬間に1回だけ発火
class MapLoadTrigger : public Trigger {
public:
	// StoryManager::load() が呼ぶ
	void arm() { armed_ = true; }
	bool check(const StoryContext& ctx) const override;

private:
	mutable bool armed_ = false;
};

#endif // TRIGGER_HPP
