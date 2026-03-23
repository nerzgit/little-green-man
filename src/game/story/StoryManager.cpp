/**
 * @file StoryManager.cpp
 * @brief ストーリーイベントの管理
 *
 * マップに紐づくストーリーイベントを読み込み、トリガー条件を満たしたらアクションを実行する。
 *   Trigger: プレイヤーの位置やアイテム所持など様々な条件を表現できる。
 *   Action:
 * 会話表示やフラグ操作など、ゲーム内で起こる様々な出来事を表現できる。
 *
 */

#include "StoryManager.hpp"

#include "../player/Player.hpp"
#include "StoryContext.hpp"
#include "action/ActionParser.hpp"
#include "trigger/MapLoadTrigger.hpp"
#include "trigger/TriggerParser.hpp"

#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

void StoryManager::load(const std::string& jsonPath) {
	events_.clear();
	actionQueue_.clear();

	if (jsonPath.empty())
		return;

	std::ifstream f(jsonPath);
	if (!f.is_open())
		return; // ストーリーなしのマップは正常

	const json root = json::parse(f);

	for (const auto& ev : root.at("events")) {
		StoryEvent e;
		e.id              = ev.at("id").get<std::string>();
		e.once            = ev.value("once", true);
		e.trigger         = parseTrigger(ev.at("trigger"));
		e.actionFactories = parseActionFactories(ev.at("actions"));
		events_.push_back(std::move(e));
	}

	// マップロード直後のイベント（MapLoadTrigger）を発火させるため、全ての
	// MapLoadTrigger を arm する
	for (auto& ev : events_) {
		if (auto* t = dynamic_cast<MapLoadTrigger*>(ev.trigger.get())) {
			t->arm();
		}
	}
}

void StoryManager::update(float dt, Player& player) {
	StoryContext ctx{player, flags_};
	// アクション実行中はトリガーチェックしない（割り込み防止）
	if (!actionQueue_.isEmpty()) {
		actionQueue_.update(dt, ctx);
		return;
	}

	for (auto& ev : events_) {
		if (ev.once && ev.fired)
			continue;
		if (!ev.trigger->check(ctx))
			continue;

		if (ev.once)
			ev.fired = true;

		// ファクトリからフレッシュなアクションを生成してキューに積む
		std::vector<std::unique_ptr<Action>> actions;
		for (const auto& factory : ev.actionFactories) {
			actions.push_back(factory());
		}
		actionQueue_.enqueue(std::move(actions));
		break; // 1フレームに起動するイベントは1つだけ
	}

	actionQueue_.update(dt, ctx);

	if (ctx.mapTransitionRequested)
		pendingEvent_ = Event::NextMap;
}

StoryManager::Event StoryManager::captureEvent() {
	Event e    = pendingEvent_;
	pendingEvent_ = Event::None;
	return e;
}
