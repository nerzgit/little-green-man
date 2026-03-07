#ifndef ACTION_HPP
#define ACTION_HPP

struct StoryContext;

// アクションの基底クラス。「何かを実行する方法」だけを知っている。
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

#endif // ACTION_HPP
