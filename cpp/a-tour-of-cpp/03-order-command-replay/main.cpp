#include "AmendOrderCommand.h"
#include "CancelOrderCommand.h"
#include "NewOrderCommand.h"
#include "OrderRegistry.h"
#include "ReplayEngine.h"

#include <cassert>
#include <memory>

int main()
{
    // registry is created first, so it will outlive the engine.
    OrderRegistry registry;
    ReplayEngine engine;

    // These are caller-owned request objects.
    Order order_101{101, 100.0, 10, Side::Buy};
    Order order_202{202, 200.0, 20, Side::Sell};

    // Record commands. Nothing has reached the registry yet.
    engine.add(std::make_unique<NewOrderCommand>(order_101));
    engine.add(std::make_unique<NewOrderCommand>(order_202));
    engine.add(std::make_unique<AmendOrderCommand>(101, 101.50, 15));
    engine.add(std::make_unique<CancelOrderCommand>(202));

    // Proves that recording commands does not execute them.
    assert(registry.find(101) == nullptr);
    assert(registry.find(202) == nullptr);

    // Execute commands in insertion order.
    engine.replay(registry);

    // New order 101 exists and was later amended.
    Order* active_101 = registry.find(101);
    assert(active_101 != nullptr);
    assert(active_101->price == 101.50);
    assert(active_101->quantity == 15);

    // Order 202 was created, then cancelled.
    assert(registry.find(202) == nullptr);
}