#pragma once

#include "PriceConsumer.h"

// Raises one alert when price moves from below the threshold
// to at or above the threshold.
class ThresholdAlertConsumer : public PriceConsumer
{
    Price threshold;
    bool was_at_or_above_threshold{false};

    int alert_count_value{0};
    Price last_alert_price_value{0.0};
    UpdateSequence last_alert_sequence_value{0};

public:
    explicit ThresholdAlertConsumer(Price threshold);

    void on_price(const PriceUpdate& update) override;

    int alert_count() const;
    Price last_alert_price() const;
    UpdateSequence last_alert_sequence() const;
};