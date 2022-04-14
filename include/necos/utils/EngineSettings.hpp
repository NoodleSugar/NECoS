#pragma once

namespace necos
{

template<class TIdAllocator, class TComponentTypeSet, class TSystemTypeSet>
struct EngineSettings
{
	using IdAllocator	   = TIdAllocator;
	using ComponentTypeSet = TComponentTypeSet;
	using SystemTypeSet	   = TSystemTypeSet;
};

} // namespace necos