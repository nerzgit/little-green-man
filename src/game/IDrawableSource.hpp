#ifndef IDRAWABLE_SOURCE_HPP
#define IDRAWABLE_SOURCE_HPP

#include "Drawable.hpp"
#include <vector>

class IDrawableSource {
public:
	virtual ~IDrawableSource()                                      = default;
	virtual void collectDrawables(std::vector<Drawable>& out) const = 0;
};

#endif // IDRAWABLE_SOURCE_HPP
