#ifndef TRIGGERS_HPP
#define TRIGGERS_HPP

// 全トリガーをまとめてインクルードする窓口。
// StoryManager.cpp のような、全種類を必要とする場所でのみ使う。
// 個々のトリガーだけが必要な場合は直接インクルードすること。
#include "AndTrigger.hpp"
#include "FlagTrigger.hpp"
#include "MapLoadTrigger.hpp"
#include "ReceiverCountTrigger.hpp"
#include "ZoneTrigger.hpp"

#endif // TRIGGERS_HPP
