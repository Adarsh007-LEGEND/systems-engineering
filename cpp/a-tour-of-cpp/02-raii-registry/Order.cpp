#include "Order.h"

// Creates the one shared counter declared in Order.h.
int Order::live_order_objects = 0;

Order::Order(OrderId id,
             Price price,
             Quantity quantity,
             Side side)
    : id{id},
      price{price},
      quantity{quantity},
      side{side}
{
    ++live_order_objects;
}

Order::Order(const Order& other)
    : id{other.id},
      price{other.price},
      quantity{other.quantity},
      side{other.side}
{
    ++live_order_objects;
}

Order::~Order()
{
    --live_order_objects;
}