[Engine]:  include/necos/Engine.hpp
[ISystem]: include/necos/ISystem.hpp

[EngineSettings]:   include/necos/utils/EngineSettings.hpp
[IdAllocator]:      include/necos/utils/EngineSettings.hpp#L9
[ComponentTypeSet]: include/necos/utils/EngineSettings.hpp#L10
[SystemTypeSet]:    include/necos/utils/EngineSettings.hpp#L11
[TypeSet]:          include/necos/utils/TypeSet.hpp#L37
[Signature]:        include/necos/utils/Signature.hpp

[id_t]:            include/necos/utils/id/Id.hpp#L9
[EntityId]:        include/necos/utils/id/Id.hpp#L10
[ComponentTypeId]: include/necos/utils/id/Id.hpp#L11
[SystemTypeId]:    include/necos/utils/id/Id.hpp#L12

[SingleUseIdAllocator]: include/necos/utils/id/allocator/SingleUseIdAllocator.hpp
[ReusableIdAllocator]:  include/necos/utils/id/allocator/ReusableIdAllocator.hpp




# NECoS

## Dependencies
* Boost : https://github.com/boostorg/boost
* EASTL : https://github.com/electronicarts/EASTL

## Description

### Architecture

The [***Engine***][Engine] class is the interface of the NECoS library.\
Through this class you can access the different managers.
- Entity manager
  - Create an **entity**
  - Delete an **entity**
- Component manager
  - Attach a **component** to an **entity**
  - Detach a **component** from an **entity**
  - Detach all the **components** from an **entity**
  - Verify if a **component** is attached to an **entity**
  - Get a **component** attached to an **entity** (Read & Write)
  - Enable a **component** attached to an **entity**
  - Disable a **component** attached to an **entity**
- System manager
  - Get a **system** (Read & Write)
  - Enable a **system**
  - Disable a **system**
  - Set a **system** priority
  - Set a **system** dependencies
  - Update all the **systems**
- Event manager
  - Subscribe an **event handler**
  - Push **event**

**Entities** are refered to by an unique [***EntityId***][EntityId].\
**Component** types are refered to by an unique [***ComponentTypeId***][ComponentTypeId] and are plain data structures.\
**Systems** types are refered to by an unique [***SystemTypeId***][SystemTypeId] and must inherit from [***ISystem***][ISystem].\
**Ids** are basically aliases of [***size_t***][id_t].

### Configuration

The [***Engine***][Engine] class must be parameterized by a ***Settings*** template class (see [***EngineSettings***][EngineSettings]).\
This class is composed of type aliases :
- [***IdAllocator***][IdAllocator]
- [***ComponentTypeSet***][ComponentTypeSet]
- [***SystemTypeSet***][SystemTypeSet]

#### IdAllocator

It is used to allocate *ids* for *entities*.\
Such an allocator must define two methods :
- *EntityId allocate()*
- *void free(EntityId)*

You can either define a custom one, or use one that is already defined :
- [***SingleUseIdAllocator***][SingleUseIdAllocator]
- [***ReusableIdAllocator***][ReusableIdAllocator]

#### ComponentTypeSet

It is used to know at compile time the number and the types of the components that are going to be used in the program.\
To define it, you can use the utility class [***TypeSet***][TypeSet].

#### SystemTypeSet

Same as for ***ComponentTypeSet***, but for *systems*.

## Glossary

### NECoS
Naive Entity Component System.

### Entity
Manipulated object within an necos-based software.

### Component
Plain data that can be attached to an entity.

### System
Update components for entities in its selection.

### Signature
Set of *ComponentTypeId* that indicates the type of components attached to an entity or required by a system.

### Archetype
Group of entities sharing the same signature.

### Selection
Group of entities that a system should update.\
Usually corresponding to the archetype matching the system signature.

### Event
Plain data that can be emitted by an [***EventEmitter***](./include/necos/event/EventEmitter.hpp).

### Event Handler
Function callback triggered by a specific type of event.

## References

- https://github.com/skypjack/entt
- https://www.guru99.com/entity-component-system.html
- https://austinmorlan.com/posts/entity_component_system/
- http://guillaume.belz.free.fr/doku.php?id=ECS