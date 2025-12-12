#pragma once

#include<bits/stdc++.h>
#include<Order.h>
#include<Trade.h>

class Orderbook
{
    public:
    


    private:
    struct OrderEntry
    {
        OrderPointer order_ { nullptr };
        OrderPointers::iterator location_;

    };

    map<Price, OrderPointers, greater<Price>> bids_;
    map<Price, OrderPointers, less<Price>> asks_;
    unordered_map<OrderId, OrderEntry> orders_;


};