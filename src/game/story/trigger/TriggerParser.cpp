#include "TriggerParser.hpp"

#include "Triggers.hpp"
#include <stdexcept>

std::unique_ptr<Trigger> parseTrigger(const nlohmann::json& j) {
	const std::string type = j.at("type").get<std::string>();

	if (type == "zone") {
		return std::make_unique<ZoneTrigger>(
		    j.at("x").get<float>(), j.at("y").get<float>(),
		    j.at("w").get<float>(), j.at("h").get<float>());
	}
	if (type == "map_load") {
		return std::make_unique<MapLoadTrigger>();
	}
	if (type == "flag") {
		return std::make_unique<FlagTrigger>(j.at("flag").get<std::string>());
	}
	if (type == "and") {
		std::vector<std::unique_ptr<Trigger>> conditions;
		for (const auto& c : j.at("conditions")) {
			conditions.push_back(parseTrigger(c));
		}
		return std::make_unique<AndTrigger>(std::move(conditions));
	}
	throw std::runtime_error("Unknown trigger type: " + type);
}
