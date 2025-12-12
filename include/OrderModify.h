#pragma once

#include<bits/stdc++.h>
#include<Order.h>
#include<Constants.h>

using namespace std;

class OrderModify
{
public:
    OrderModify(OrderId orderId, Side side, Price price, Quantity quantity);
        
    // writing the API calls
    OrderId GetOrderId() const;
    Side GetSide() const;
    Price GetPrice() const;
    Quantity GetQuantity() const;

    OrderPointer ToOrderPointer(OrderType type) const;
    

private:
    OrderId orderId_;
    Side side_;
    Price price_;
    Quantity quantity_;
};