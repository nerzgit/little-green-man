#ifndef ACTION_PARSER_HPP
#define ACTION_PARSER_HPP

#include "../StoryEvent.hpp"
#include <nlohmann/json.hpp>
#include <vector>

// JSON 配列からアクションファクトリのリストを生成する。
std::vector<ActionFactory> parseActionFactories(const nlohmann::json& j);

#endif // ACTION_PARSER_HPP
