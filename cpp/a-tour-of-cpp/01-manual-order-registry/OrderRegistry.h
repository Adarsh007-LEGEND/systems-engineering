#pragma once

#include "Order.h"
#include <map>

class OrderRegistry {
    std::map<OrderId, Order*> registry;

public:
    OrderRegistry() = default;
    // means the compiler-generated constructor is sufficient
    // I can't figure out what to initialize, I mean there aren't any data members I can initialize
    // so don't initialize

    // Headers contain only WHAT, not HOW.
    void create(const Order& order);
    // The user is constructing an order and the registry is only responsible for CREATING an order, not constructing it.
    // The caller never owns the heap object inside the registry, i.e. what registry[id] points to (an Order* on the heap).

    Order* find(OrderId id);
    // returns a borrowed pointer
    // caller must not delete it
    // the pointer becomes invalid after cancel(id) or registry deletion;

    void cancel(OrderId id);
    // deletes the registry-owned order from the heap

    ~OrderRegistry();
    // You don't even need to know how to delete the map.
    // Data members are destroyed automatically by the compiler.
    // Our responsibility is only to free the heap memory (Order objects) that the registry owns.

    OrderRegistry(const OrderRegistry&) = delete;
    OrderRegistry& operator=(const OrderRegistry&) = delete;
};