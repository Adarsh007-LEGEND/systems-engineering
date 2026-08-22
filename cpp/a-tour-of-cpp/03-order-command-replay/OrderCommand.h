#pragma once

class OrderRegistry;

// Abstract interface for one replayable operation.
class OrderCommand {
public:
    // Required because derived command objects are owned
    // and destroyed through OrderCommand pointers.
    virtual ~OrderCommand() = default;

    // Every concrete command must define its own registry operation.
    virtual void apply(OrderRegistry& registry) = 0;
};