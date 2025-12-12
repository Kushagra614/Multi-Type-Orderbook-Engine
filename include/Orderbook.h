#pragma once

#include<bits/stdc++.h>
#include"Order.h"
#include"Trade.h"
#include"OrderbookLevelInfos.h"
#include"OrderModify.h"

using namespace std;

class Orderbook
{
    public:
    Trades AddOrder(OrderPointer order);
    void CancelOrder(OrderId orderId);
    Trades MatchOrders(OrderModify order);
    size_t Size() const;
    OrderbookLevelInfos GetOrderInfos() const;


    private:
    struct OrderEntry
    {
        OrderPointer order_ { nullptr };
        OrderPointers::iterator location_;

    };

    map<Price, OrderPointers, greater<Price>> bids_;
    map<Price, OrderPointers, less<Price>> asks_;
    unordered_map<OrderId, OrderEntry> orders_;

    bool CanMatch(Side side, Price price) const;
    Trades MatchOrders();


};