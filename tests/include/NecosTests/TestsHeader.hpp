#pragma once

#include <catch2/catch.hpp>

#include "Resources/Components.hpp"
#include "Resources/EngineSettings.hpp"
#include "Resources/Events.hpp"
#include "Resources/Systems.hpp"

#include <necos/Engine.hpp>
#include <necos/event/EventEmitter.hpp>
#include <necos/internal/ComponentContainer.hpp>
#include <necos/manager/ComponentManager.hpp>
#include <necos/manager/EntityManager.hpp>
#include <necos/manager/EventManager.hpp>
#include <necos/manager/SystemManager.hpp>
#include <necos/utils/Signature.hpp>
#include <necos/utils/TypeSet.hpp>
#include <necos/utils/id/Id.hpp>
#include <necos/utils/id/allocator/ReusableIdAllocator.hpp>
#include <necos/utils/id/allocator/SingleUseIdAllocator.hpp>

#include <vector>

using Catch::Matchers::VectorContains;

using ReusableIdAllocator  = necos::ReusableIdAllocator;
using SingleUseIdAllocator = necos::SingleUseIdAllocator;

using Engine		   = necos::Engine<EngineSettings>;
using EventManager	   = necos::EventManager;
using EventEmitter	   = necos::EventEmitter;
using EntityManager	   = necos::EntityManager<SingleUseIdAllocator>;
using ComponentManager = necos::ComponentManager<ComponentTypeSet>;
using SystemManager	   = necos::SystemManager<ComponentTypeSet, SystemTypeSet>;

using Signature		 = necos::Signature<ComponentTypeSet>;
using BasicContainer = necos::ComponentContainer<BasicComponent>;
using CustomTypeSet	 = necos::TypeSet<int, float, char>;

using Id			  = necos::id_t;
using EntityId		  = necos::EntityId;
using ComponentTypeId = necos::ComponentTypeId;
using SystemTypeId	  = necos::SystemTypeId;