#ifndef ZONE_TRIGGER_HPP
#define ZONE_TRIGGER_HPP

#include "Trigger.hpp"

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

#endif // ZONE_TRIGGER_HPP
