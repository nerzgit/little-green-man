#ifndef TEXTURE_CACHE_HPP
#define TEXTURE_CACHE_HPP

#include "Texture.hpp"
#include <memory>
#include <string>
#include <unordered_map>

// パスをキーにテクスチャを共有するキャッシュ。
// weak_ptr で保持するため、参照がなくなったテクスチャは自動的に解放される。
class TextureCache {
public:
	static std::shared_ptr<Texture> get(const std::string& path);

private:
	static std::unordered_map<std::string, std::weak_ptr<Texture>> cache_;
};

#endif // TEXTURE_CACHE_HPP
