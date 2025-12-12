#pragma once

#include<bits/stdc++.h>
#include<Constants.h>

//Now we will be working on the matching of the trades in the orderbook
struct TradeInfo
{
    OrderId orderid_;
    Price price_;
    Quantity quantity_;
};

