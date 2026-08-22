#pragma once

#include "OrderCommand.h"

#include <memory>
#include <vector>

class OrderRegistry;

class ReplayEngine {
    // Owns every command object, in the exact order it was added.
    std::vector<std::unique_ptr<OrderCommand>> commands;

public:
    // Takes ownership of one concrete command.
    void add(std::unique_ptr<OrderCommand> command);

    // Executes stored commands from first to last.
    void replay(OrderRegistry& registry);
};