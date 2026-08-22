#pragma once

#include "Order.h"
#include "OrderCommand.h"

class CancelOrderCommand : public OrderCommand {
    OrderId id;

public:
    CancelOrderCommand(OrderId id_to_cancel);

    void apply(OrderRegistry& registry) override;
};