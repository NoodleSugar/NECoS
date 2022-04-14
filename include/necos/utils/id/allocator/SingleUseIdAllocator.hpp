#pragma once

#include <cstddef>

namespace necos
{

class SingleUseIdAllocator
{
public:
	size_t allocate() { return m_topId++; }
	void   free(size_t) {}

private:
	size_t m_topId = 0;
};

} // namespace necos