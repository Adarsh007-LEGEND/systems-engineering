#include "OrderRegistry.h"

#include <cassert>

int main()
{
    // No Order object exists at program start.
    assert(Order::live_order_objects == 0);

    {
        OrderRegistry registry;

        Order caller_order{1, 100.5, 10, Side::Buy};
        assert(Order::live_order_objects == 1);

        registry.create(caller_order);
        assert(Order::live_order_objects == 2);

        Order* found_order = registry.find(1);

        assert(found_order != nullptr);
        assert(found_order != &caller_order);
        assert(found_order->id == 1);
        assert(found_order->price == 100.5);
        assert(found_order->quantity == 10);
        assert(found_order->side == Side::Buy);

        registry.cancel(1);

        // The registry-owned heap copy died.
        // caller_order still exists.
        assert(Order::live_order_objects == 1);
        assert(registry.find(1) == nullptr);
    }

    // caller_order and registry are both out of scope.
    assert(Order::live_order_objects == 0);

    return 0;
}