#pragma once

#include "Order.h"
#include <map>
#include <memory>

class OrderRegistry {
    // Every map entry uniquely owns one heap Order.
    std::map<OrderId, std::unique_ptr<Order>> registry;

public:
    // The compiler default-constructs registry as an empty map.
    OrderRegistry() = default;

    // Creates an independent heap copy of order.
    // The registry owns that copy after this call.
    // Precondition: order.id is not already active.
    void create(const Order& order);

    // Returns a borrowed pointer, or nullptr if id is absent.
    // Caller must never delete it.
    // A non-null result becomes invalid after cancel(id)
    // or OrderRegistry destruction.
    Order* find(OrderId id);

    // Removes the owning map entry.
    // Its unique_ptr automatically destroys the heap Order.
    // Precondition: id is currently active.
    void cancel(OrderId id);


    void amend(OrderId id, Price new_price, Quantity new_quantity);
    // No user-written destructor:
    // registry destroys itself, then its unique_ptrs destroy their Orders.

    // One registry must not be copied into a second owner.
    // unique_ptr also makes copying impossible; these lines state the API intent.
    OrderRegistry(const OrderRegistry&) = delete;
    OrderRegistry& operator=(const OrderRegistry&) = delete;
};