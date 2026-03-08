#ifndef TRIGGER_PARSER_HPP
#define TRIGGER_PARSER_HPP

#include "Trigger.hpp"
#include <memory>
#include <nlohmann/json.hpp>

// JSON オブジェクトからトリガーを生成する。
// "and" は再帰的に自身を呼ぶ。
std::unique_ptr<Trigger> parseTrigger(const nlohmann::json& j);

#endif // TRIGGER_PARSER_HPP
