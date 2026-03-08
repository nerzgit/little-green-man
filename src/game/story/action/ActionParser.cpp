#include "ActionParser.hpp"

#include "Actions.hpp"
#include <stdexcept>

std::vector<ActionFactory> parseActionFactories(const nlohmann::json& j) {
	std::vector<ActionFactory> factories;

	for (const auto& item : j) {
		const std::string type = item.at("type").get<std::string>();

		if (type == "dialogue") {
			auto lines = item.at("lines").get<std::vector<std::string>>();
			factories.push_back([lines]() -> std::unique_ptr<Action> {
				return std::make_unique<DialogueAction>(lines);
			});
		} else if (type == "set_flag") {
			auto flag = item.at("flag").get<std::string>();
			factories.push_back([flag]() -> std::unique_ptr<Action> {
				return std::make_unique<SetFlagAction>(flag);
			});
		} else if (type == "wait") {
			float seconds = item.at("seconds").get<float>();
			factories.push_back([seconds]() -> std::unique_ptr<Action> {
				return std::make_unique<WaitAction>(seconds);
			});
		} else {
			throw std::runtime_error("Unknown action type: " + type);
		}
	}
	return factories;
}
