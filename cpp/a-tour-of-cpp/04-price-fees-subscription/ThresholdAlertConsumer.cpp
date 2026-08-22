#include "ThresholdAlertConsumer.h"

ThresholdAlertConsumer::ThresholdAlertConsumer(Price threshold)
    : threshold{threshold}
{
}

void ThresholdAlertConsumer::on_price(const PriceUpdate& update)
{
    const bool currently_at_or_above_threshold = update.price >= threshold;

    // Alert only on an upward threshold crossing.
    if (currently_at_or_above_threshold && !was_at_or_above_threshold) {
        ++alert_count_value;
        last_alert_price_value = update.price;
        last_alert_sequence_value = update.sequence_number;
    }

    // Remember the current state for the next update.
    was_at_or_above_threshold = currently_at_or_above_threshold;
}

int ThresholdAlertConsumer::alert_count() const
{
    return alert_count_value;
}

Price ThresholdAlertConsumer::last_alert_price() const
{
    return last_alert_price_value;
}

UpdateSequence ThresholdAlertConsumer::last_alert_sequence() const
{
    return last_alert_sequence_value;
}