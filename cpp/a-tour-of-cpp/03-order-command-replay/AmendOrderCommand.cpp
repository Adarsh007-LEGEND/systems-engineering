#include "AmendOrderCommand.h"
#include "OrderRegistry.h"

AmendOrderCommand::AmendOrderCommand(OrderId id_to_amend,
                                     Price price_after_amend,
                                     Quantity quantity_after_amend)
    : id{id_to_amend},
      new_price{price_after_amend},
      new_quantity{quantity_after_amend}
{
}

void AmendOrderCommand::apply(OrderRegistry& registry)
{
    registry.amend(id, new_price, new_quantity);
}