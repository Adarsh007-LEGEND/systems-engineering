enum class Side
{
    Buy,
    Sell
};

using OrderId = int;
using Price = double;
using Quantity = int;

struct Order
{
    OrderId id;
    Price price;
    Quantity quantity;
    Side side;

    Order(OrderId id,
          Price price,
          Quantity quantity,
          Side side)
        : id{id},
          price{price},
          quantity{quantity},
          side{side}
    {
    }
};