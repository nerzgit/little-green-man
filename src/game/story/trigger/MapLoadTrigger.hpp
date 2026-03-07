#ifndef MAP_LOAD_TRIGGER_HPP
#define MAP_LOAD_TRIGGER_HPP

#include "Trigger.hpp"

// マップがロードされた瞬間に1回だけ発火。
// StoryManager::load() が arm() を呼び、次の update() で発火する。
class MapLoadTrigger : public Trigger {
public:
	void arm() { armed_ = true; }

	bool check(const StoryContext&) const override {
		if (armed_) {
			armed_ = false;
			return true;
		}
		return false;
	}

private:
	mutable bool armed_ = false;
};

#endif // MAP_LOAD_TRIGGER_HPP
