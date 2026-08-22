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

    //one shared test counter for every live object
    static int live_order_objects;

    OrderId id;
    Price price;
    Quantity quantity;
    Side side;

    Order(OrderId id,
          Price price,
          Quantity quantity,
          Side side);

    Order(const Order& other);

    ~Order();
};