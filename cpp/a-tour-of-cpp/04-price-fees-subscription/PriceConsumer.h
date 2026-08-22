#pragma once

#include "PriceUpdate.h"

// Common contract for any object that wants price updates.
//
// PriceFeed will notify consumers through this interface.
// A consumer does not own PriceFeed and must not retain a reference
// to the borrowed PriceUpdate after on_price() returns.
class PriceConsumer
{
public:
    virtual ~PriceConsumer() = default;
    //parent class

    virtual void on_price(const PriceUpdate& update) = 0;
    //const here because we don't want the consumer to modify pur incoming object
};