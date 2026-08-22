#include "CancelOrderCommand.h"
#include "OrderRegistry.h"

CancelOrderCommand::CancelOrderCommand(OrderId id_to_cancel)
    : id{id_to_cancel}
{
}

void CancelOrderCommand::apply(OrderRegistry& registry)
{
    registry.cancel(id);
}