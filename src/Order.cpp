#include "Order.h"
#include <stdexcept>   // for std::logic_error

Order::Order(OrderType orderType, OrderId orderId, Side side, Price price, Quantity quantity)
    : orderType_{orderType},
      orderId_{orderId},
      side_{side},
      price_{price},
      initialQuantity_{quantity},
      remainingQuantity_{quantity}
{}

OrderType Order::GetOrderType() const { return orderType_; }
OrderId Order::GetOrderId() const { return orderId_; }
Side Order::GetSide() const { return side_; }
Price Order::GetPrice() const { return price_; }

Quantity Order::GetInitialQuantity() const { return initialQuantity_; }
Quantity Order::GetRemainingQuantity() const { return remainingQuantity_; }
Quantity Order::GetFilledQuantity() const { return initialQuantity_ - remainingQuantity_; }

bool Order::isFilled() const {
    return remainingQuantity_ == 0;
}

void Order::Fill(Quantity quantity) {
    if (quantity > remainingQuantity_) {
        throw std::logic_error("Order cannot be filled beyond remaining quantity");
    }

    remainingQuantity_ -= quantity;
}
