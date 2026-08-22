#include "PriceFeed.h"

#include<cassert>

void PriceFeed::subscribe(const std::shared_ptr<PriceConsumer>& consumer){

    // a suscription must refer to a real externally owned consumer;
    assert(consumer != nullptr);

    for(const auto& subscription : subscriptions){
        //this auto is actually of type weak_ptr
        
        const auto existing_consumer = subscription.lock();
        //now this becomes a temporary shared_ptr if the weak_ptr was still pointing to a live consumer

        assert(existing_consumer != consumer);
    }

    //convert the caller's strong owner int a non-owning observation entry.
    //This does not increase the consumer's strong owner count, so we can push it safely.
    subscriptions.push_back(consumer);
}

void PriceFeed::unsubscribe(const std::shared_ptr<PriceConsumer>& consumer){
    //The consumer must be real, currently alive
    assert(consumer != nullptr);

    for(auto it = subscriptions.begin(); it != subscriptions.end(); ++it){
        //temporarily attain strong access if the consumer is still active
        const auto existing_consumer = it->lock();

        if(existing_consumer == consumer){
            //here we will remove the PriceFeed's observation i.e. the weak_ptr entry only
            //The externally owned consumer is still active

            subscriptions.erase(it);
            return;
        }
    }

    assert(false && "Consumer is not subscribed");
}


void PriceFeed::publish(Price price)
{
    // This update exists only for this publish() call.
    PriceUpdate update{price, next_sequence};
    ++next_sequence;

    // Do not increment after erase(): vector::erase() already returns
    // the next valid iterator.
    for (auto it = subscriptions.begin(); it != subscriptions.end();) {
        // lock() gives a temporary strong owner if the consumer is alive.
        //which is required to perform any operation on the alive consumer, which otherwise may become dead inbetween a function call
        //because weak_ptr only tells that this consumer "MAY BE ALIVE"
        const auto live_consumer = it->lock();

        if (live_consumer == nullptr) {
            // The consumer died elsewhere. Remove the stale observer entry.
            it = subscriptions.erase(it);
            continue;
        }

        // Safe: live_consumer keeps the consumer alive for this callback.
        live_consumer->on_price(update);

        ++it;
    }
}