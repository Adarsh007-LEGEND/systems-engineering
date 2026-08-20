#include "OrderRegistry.h"
#include <cassert>

int main()
{
    OrderRegistry registry;

    Order caller_order{1, 100.5, 10, Side::Buy};

    // Nothing exists before creation.
    assert(registry.find(1) == nullptr);

    registry.create(caller_order);

    Order* found_order = registry.find(1);

    // The registry now contains an Order with this ID.
    assert(found_order != nullptr);

    // The registry owns an independent heap copy, not caller_order itself.
    assert(found_order != &caller_order);

    // The stored copy has the same values as the caller's original Order.
    assert(found_order->id == 1);
    assert(found_order->price == 100.5);
    assert(found_order->quantity == 10);
    assert(found_order->side == Side::Buy);

    registry.cancel(1);

    // The entry no longer exists.
    assert(registry.find(1) == nullptr);

    return 0;
}