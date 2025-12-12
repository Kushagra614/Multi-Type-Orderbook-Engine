#pragma once

#include <map>
#include <list>
#include <unordered_map>

#include "Order.h"
#include "OrderModify.h"
#include "Trade.h"
#include "OrderbookLevelInfos.h"

class Orderbook {
public:
    Orderbook();

    Trades AddOrder(OrderPointer order);
    void CancelOrder(OrderId id);
    Trades MatchOrders(OrderModify mod);

    size_t Size() const;
    OrderbookLevelInfos GetOrderInfos() const;

private:
    struct OrderEntry {
        OrderPointer order_;
        OrderPointers::iterator location_;
    };

    std::map<Price, OrderPointers, std::greater<Price>> bids_;
    std::map<Price, OrderPointers, std::less<Price>>    asks_;
    std::unordered_map<OrderId, OrderEntry> orders_;

    bool CanMatch(Side side, Price price) const;
    Trades MatchOrders();
};
