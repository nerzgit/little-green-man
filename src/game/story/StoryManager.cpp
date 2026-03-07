#include "StoryManager.hpp"

#include "trigger/Triggers.hpp"
#include "action/Actions.hpp"

#include <fstream>
#include <nlohmann/json.hpp>
#include <stdexcept>

using json = nlohmann::json;

// JSON解析ロジックをここに閉じ込める。ヘッダへの漏れなし。
namespace {

std::unique_ptr<Trigger> parseTrigger(const json& j) {
	const std::string type = j.at("type").get<std::string>();

	if (type == "zone") {
		return std::make_unique<ZoneTrigger>(
		  j.at("x").get<float>(), j.at("y").get<float>(),
		  j.at("w").get<float>(), j.at("h").get<float>());
	}
	if (type == "map_load") {
		return std::make_unique<MapLoadTrigger>();
	}
	throw std::runtime_error("Unknown trigger type: " + type);
}

// Action をその場で生成するのではなく「生成方法」をラムダで返す。
// これにより once=false のイベントでも毎回フレッシュなインスタンスを使える。
std::vector<ActionFactory> parseActionFactories(const json& arr) {
	std::vector<ActionFactory> factories;

	for (const auto& j : arr) {
		const std::string type = j.at("type").get<std::string>();

		if (type == "dialogue") {
			auto lines = j.at("lines").get<std::vector<std::string>>();
			factories.push_back([lines]() -> std::unique_ptr<Action> {
				return std::make_unique<DialogueAction>(lines);
			});
		} else if (type == "set_flag") {
			auto flag = j.at("flag").get<std::string>();
			factories.push_back([flag]() -> std::unique_ptr<Action> {
				return std::make_unique<SetFlagAction>(flag);
			});
		} else if (type == "wait") {
			float seconds = j.at("seconds").get<float>();
			factories.push_back([seconds]() -> std::unique_ptr<Action> {
				return std::make_unique<WaitAction>(seconds);
			});
		} else {
			throw std::runtime_error("Unknown action type: " + type);
		}
	}
	return factories;
}

} // namespace

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

	// MapLoadTrigger を arm しておく（load 直後の update で発火させるため）
	for (auto& ev : events_) {
		if (auto* t = dynamic_cast<MapLoadTrigger*>(ev.trigger.get())) {
			t->arm();
		}
	}
}

void StoryManager::update(float dt, StoryContext& ctx) {
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
}
