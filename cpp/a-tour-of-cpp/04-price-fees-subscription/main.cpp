#include "PriceFeed.h"
#include "RecordingConsumer.h"
#include "ThresholdAlertConsumer.h"

#include <cassert>
#include <memory>

int main()
{
    PriceFeed feed;

    auto terminal_view = std::make_shared<RecordingConsumer>("terminal");
    auto risk_view = std::make_shared<RecordingConsumer>("risk");

    // Different derived type: it has different on_price() behaviour.
    // justifies the use of inheritance and method overridding
    auto threshold_alert = std::make_shared<ThresholdAlertConsumer>(103.00);

    feed.subscribe(terminal_view);
    feed.subscribe(risk_view);
    feed.subscribe(threshold_alert);

    // Everyone receives the update, but no threshold crossing happened.
    feed.publish(101.25);

    assert(terminal_view->update_count() == 1);
    assert(risk_view->update_count() == 1);
    assert(threshold_alert->alert_count() == 0);

    // Price crosses upward through 103.00.
    feed.publish(105.00);

    assert(terminal_view->update_count() == 2);
    assert(risk_view->update_count() == 2);
    assert(threshold_alert->alert_count() == 1);
    assert(threshold_alert->last_alert_price() == 105.00);
    assert(threshold_alert->last_alert_sequence() == 2);

    // Still above threshold: no second crossing, hence no second alert.
    feed.publish(106.00);
    assert(threshold_alert->alert_count() == 1);

    // Move below the threshold, resetting the alert condition.
    feed.publish(100.00);
    assert(threshold_alert->alert_count() == 1);

    // Cross upward again: second alert.
    feed.publish(104.00);
    assert(threshold_alert->alert_count() == 2);
    assert(threshold_alert->last_alert_sequence() == 5);

    // Prove PriceFeed does not keep the alert consumer alive.
    std::weak_ptr<ThresholdAlertConsumer> alert_observer = threshold_alert;
    threshold_alert.reset();

    assert(alert_observer.expired());

    // Expired alert entry is safely removed; live consumers still receive updates.
    feed.publish(102.50);

    assert(terminal_view->update_count() == 6);
    assert(risk_view->update_count() == 6);

    // Unsubscribing stops delivery but does not destroy risk_view.
    feed.unsubscribe(risk_view);
    feed.publish(103.75);

    assert(terminal_view->update_count() == 7);
    assert(risk_view->update_count() == 6);

    return 0;
}