#pragma once

#include "Order.h"

class OrderModify {
public:
    OrderModify(OrderId id, Side side, Price price, Quantity qty)
        : orderId_(id), side_(side), price_(price), quantity_(qty)
    {}

    OrderId GetOrderId() const { return orderId_; }
    Side GetSide() const { return side_; }
    Price GetPrice() const { return price_; }
    Quantity GetQuantity() const { return quantity_; }

    OrderPointer ToOrderPointer(OrderType type) const {
        return std::make_shared<Order>(type, orderId_, side_, price_, quantity_);
    }

private:
    OrderId orderId_;
    Side side_;
    Price price_;
    Quantity quantity_;
};
