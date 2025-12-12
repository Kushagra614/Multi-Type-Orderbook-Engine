#pragma once

#include <memory>
#include <list>
#include <stdexcept>

#include "Usings.h"
#include "Side.h"
#include "OrderType.h"

class Order {
public:
    Order(OrderType orderType, OrderId orderId, Side side, Price price, Quantity quantity)
        : orderType_(orderType),
          orderId_(orderId),
          side_(side),
          price_(price),
          initialQuantity_(quantity),
          remainingQuantity_(quantity)
    {}

    OrderType GetOrderType() const { return orderType_; }
    OrderId GetOrderId() const { return orderId_; }
    Side GetSide() const { return side_; }
    Price GetPrice() const { return price_; }

    Quantity GetInitialQuantity() const { return initialQuantity_; }
    Quantity GetRemainingQuantity() const { return remainingQuantity_; }
    Quantity GetFilledQuantity() const { return initialQuantity_ - remainingQuantity_; }

    bool isFilled() const { return remainingQuantity_ == 0; }

    void Fill(Quantity qty) {
        if (qty > remainingQuantity_) {
            throw std::logic_error("Attempt to over-fill order");
        }
        remainingQuantity_ -= qty;
    }

private:
    OrderType orderType_;
    OrderId orderId_;
    Side side_;
    Price price_;
    Quantity initialQuantity_;
    Quantity remainingQuantity_;
};

using OrderPointer  = std::shared_ptr<Order>;
using OrderPointers = std::list<OrderPointer>;
