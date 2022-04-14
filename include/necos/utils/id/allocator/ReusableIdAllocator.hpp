#pragma once

#include <EASTL/priority_queue.h>

#include <cstddef>

namespace necos
{

class ReusableIdAllocator
{
public:
	size_t allocate()
	{
		if(m_idQueue.empty())
			return m_topId++;

		size_t id = m_idQueue.top();
		m_idQueue.pop();

		return id;
	}

	void free(size_t id)
	{
		m_idQueue.push(id);
	}

private:
	size_t m_topId = 0;

	eastl::priority_queue<size_t> m_idQueue;
};

} // namespace necos