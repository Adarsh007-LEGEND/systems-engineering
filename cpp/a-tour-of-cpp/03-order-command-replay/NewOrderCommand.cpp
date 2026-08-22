#include "NewOrderCommand.h"

#include "OrderRegistry.h"

NewOrderCommand::NewOrderCommand(const Order& order_to_create) : order{order_to_create}
{
}

void NewOrderCommand::apply(OrderRegistry& registry)
{
    // The registry creates and owns its own separate heap copy.
    registry.create(order);
}