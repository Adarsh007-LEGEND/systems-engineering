#pragma once

#include "Order.h"
#include "OrderCommand.h"

class AmendOrderCommand : public OrderCommand {
    OrderId id;
    Price new_price;
    Quantity new_quantity;

public:
    AmendOrderCommand(OrderId id_to_amend,
                      Price price_after_amend,
                      Quantity quantity_after_amend);

    void apply(OrderRegistry& registry) override;
};