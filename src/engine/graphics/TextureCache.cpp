#include "TextureCache.hpp"

std::unordered_map<std::string, std::weak_ptr<Texture>> TextureCache::cache_;

std::shared_ptr<Texture> TextureCache::get(const std::string& path) {
	auto it = cache_.find(path);
	if (it != cache_.end()) {
		if (auto tex = it->second.lock())
			return tex;
	}
	auto tex    = std::make_shared<Texture>(path);
	cache_[path] = tex;
	return tex;
}
