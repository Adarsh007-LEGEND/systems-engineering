#pragma once

using Price = double;

//the consumers can identify the order of published events
using UpdateSequence = int;

// Plain event-data object created by PriceFeed for one publish() call.
// It carries information to consumers; it owns nothing.
//
// Consumers borrow this object only while on_price() is running.
// If they need its data later, they copy the fields they need.
struct PriceUpdate{
    Price price;
    UpdateSequence sequence_number;

    PriceUpdate(Price price, UpdateSequence sequence_number) : price {price}, sequence_number {sequence_number}
    {

    }
};