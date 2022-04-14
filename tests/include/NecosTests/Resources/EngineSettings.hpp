#pragma once

#include <necos/utils/EngineSettings.hpp>
#include <necos/utils/TypeSet.hpp>
#include <necos/utils/id/allocator/SingleUseIdAllocator.hpp>

class EmptyComponent;
class BasicComponent;
using ComponentTypeSet = necos::TypeSet<EmptyComponent, BasicComponent>;

class EmptySystem;
class BasicSystem;
using SystemTypeSet = necos::TypeSet<EmptySystem, BasicSystem>;

using EngineSettings =
 necos::EngineSettings<
  necos::SingleUseIdAllocator,
  ComponentTypeSet,
  SystemTypeSet>;