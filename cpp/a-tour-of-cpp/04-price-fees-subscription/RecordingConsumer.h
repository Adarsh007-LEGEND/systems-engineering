#pragma once

#include "PriceConsumer.h"

#include <string>

// Simple concrete consumer used to test PriceFeed behavior.
//
// It does not own PriceFeed or a PriceUpdate.
// It copies the values it needs from each received update.
class RecordingConsumer : public PriceConsumer
{
    std::string name;
    int received_update_count{0};
    Price last_seen_price{0.0};
    UpdateSequence last_seen_sequence{0};

public:
    explicit RecordingConsumer(const std::string& consumer_name);

    void on_price(const PriceUpdate& update) override;

    // Read-only query: returns how many updates this consumer has received.
    // Does not modify this RecordingConsumer object.
    int update_count() const;

    // Read-only query: returns the most recently recorded price.
    // Does not modify this RecordingConsumer object.
    Price last_price() const;

    // Read-only query: returns the sequence number of the latest update.
    // Does not modify this RecordingConsumer object.
    UpdateSequence last_sequence() const;
};