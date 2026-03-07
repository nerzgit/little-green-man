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

#endif // TRIGGER_HPP
