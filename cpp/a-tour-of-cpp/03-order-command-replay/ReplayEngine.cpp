#include "ReplayEngine.h"
#include "OrderRegistry.h"

#include <cassert>
#include <utility>

void ReplayEngine::add(std::unique_ptr<OrderCommand> command)
{
    // A replayable command must point to a real command object.
    assert(command != nullptr);

    // Transfer ownership from this function's parameter into the vector.
    commands.push_back(std::move(command));
}

void ReplayEngine::replay(OrderRegistry& registry)
{
    // The engine knows only the common OrderCommand interface.
    for (const auto& command : commands) {
        command->apply(registry);
    }
}