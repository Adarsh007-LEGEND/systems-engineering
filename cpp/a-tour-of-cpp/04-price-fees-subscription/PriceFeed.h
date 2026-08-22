#pragma once

#include "PriceConsumer.h"
#include "PriceUpdate.h"

#include <memory>
#include <vector>

// Publishes PriceUpdate events to currently live subscribed consumers.
//
// PriceFeed owns the subscription list, but it does not own consumers.
// Each weak_ptr is only an observation path and must be locked before use.
class PriceFeed
{
    // Persistent subscription relationships, kept in subscription order.
    // These do not keep consumers alive.
    std::vector<std::weak_ptr<PriceConsumer>> subscriptions;

    // Gives every PriceUpdate a deterministic order.
    UpdateSequence next_sequence{1};

public:
    // Records a non-owning subscription for an externally owned consumer.
    void subscribe(const std::shared_ptr<PriceConsumer>& consumer);

    // Stops observing this still-live consumer.
    // Does not destroy the consumer.
    void unsubscribe(const std::shared_ptr<PriceConsumer>& consumer);

    // Creates one short-lived PriceUpdate and notifies live consumers.
    void publish(Price price);
};