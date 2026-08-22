#include "OrderRegistry.h"
#include <cassert>
#include <utility>

void OrderRegistry::create(const Order& order)
{
    const OrderId id = order.id;

    // A duplicate active ID means the caller broke create()'s contract.
    assert(registry.find(id) == registry.end());

    // Make a separate heap Order. The registry owns this copy.
    auto owned_order = std::make_unique<Order>(order);

    // this is illegal as we cannot copy unique pointers as copy construct and assignment is disabled in unique_ptr
    // registry[id] = registry_order; //this is assign,ent so not allowed

    registry[id] = std::move(owned_order);
}

Order* OrderRegistry::find(OrderId id)
{
    auto it = registry.find(id);

    // Absence is a normal lookup result.
    if (it == registry.end()) {
        return nullptr;
    }

    // Borrowed pointer only: caller may use it, but must never delete it.
    return it->second.get();
}

void OrderRegistry::cancel(OrderId id)
{
    auto it = registry.find(id);

    // cancel() requires an active order with this ID.
    assert(it != registry.end());

    // this time we only need to delete the map unique pointer which would automatically delete the heap object owned by it, this is RAII
    registry.erase(it);
}

// since the map gets automatically destroyed, its unique pointers will also get destroyed, thereby destroying the heap object
/*OrderRegistry::~OrderRegistry()
{
    // The map destroys itself automatically.
    // We only need to destroy the heap Orders it points to.
    for (const auto& entry : registry) {
        delete entry.second;
    }
}*/