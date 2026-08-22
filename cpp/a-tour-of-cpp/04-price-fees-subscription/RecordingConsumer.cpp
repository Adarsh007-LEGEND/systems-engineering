#include "RecordingConsumer.h"

RecordingConsumer::RecordingConsumer(const std::string& consumer_name)
    : name{consumer_name}
{
}

void RecordingConsumer::on_price(const PriceUpdate& update)
{
    last_seen_price = update.price;
    last_seen_sequence = update.sequence_number;
    ++received_update_count;
}

int RecordingConsumer::update_count() const
{
    return received_update_count;
}

Price RecordingConsumer::last_price() const
{
    return last_seen_price;
}

UpdateSequence RecordingConsumer::last_sequence() const
{
    return last_seen_sequence;
}