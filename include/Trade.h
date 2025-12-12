#pragma once

#include "Usings.h"
#include <vector>

struct TradeInfo {
    OrderId orderid_;
    Price price_;
    Quantity quantity_;
};

class Trade {
public:
    Trade(const TradeInfo& bid, const TradeInfo& ask)
        : bid_(bid), ask_(ask)
    {}

    const TradeInfo& GetBidTrade() const { return bid_; }
    const TradeInfo& GetAskTrade() const { return ask_; }

private:
    TradeInfo bid_;
    TradeInfo ask_;
};

using Trades = std::vector<Trade>;
