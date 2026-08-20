#include "OrderRegistry.h"
#include <cassert>

void OrderRegistry::create(const Order& order)
{
    const OrderId id = order.id;

    // A duplicate active ID means the caller broke create()'s contract.
    assert(registry.find(id) == registry.end());

    // Make a separate heap Order. The registry owns this copy.
    Order* registry_order = new Order(order);

    // registry[id] now points to the independently owned heap Order.
    registry[id] = registry_order;
}

Order* OrderRegistry::find(OrderId id)
{
    auto it = registry.find(id);

    // Absence is a normal lookup result.
    if (it == registry.end()) {
        return nullptr;
    }

    // Borrowed pointer only: caller may use it, but must never delete it.
    return it->second;
}

void OrderRegistry::cancel(OrderId id)
{
    auto it = registry.find(id);

    // cancel() requires an active order with this ID.
    assert(it != registry.end());

    // First destroy the heap Order owned by the registry.
    delete it->second;

    // Then remove the key-pointer pair from the map.
    registry.erase(it);
}

OrderRegistry::~OrderRegistry()
{
    // The map destroys itself automatically.
    // We only need to destroy the heap Orders it points to.
    for (const auto& entry : registry) {
        delete entry.second;
    }
}