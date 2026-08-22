#pragma once

#include "Order.h"
#include "OrderCommand.h"

class NewOrderCommand : public OrderCommand {
    // This command owns its own Order payload as value data.
    Order order;
    //this needs to store the order object as it will be used in the overriden function to create an entry in the registry

public:
    NewOrderCommand(const Order& order_to_create);

    // Fulfils OrderCommand's apply() promise.
    // override asks the compiler to verify that it really overrides it.
    void apply(OrderRegistry& registry) override;
};