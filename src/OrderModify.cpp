#include "OrderModify.h"

OrderModify::OrderModify(OrderId orderId, Side side, Price price, Quantity quantity)
        : orderId_{orderId}, side_{side}, price_{price}, quantity_{quantity}

    { }

    // writing the API calls
    OrderId OrderModify::GetOrderId() const { return orderId_; }
    Side OrderModify::GetSide() const { return side_; }
    Price OrderModify::GetPrice() const { return price_; }
    Quantity OrderModify::GetQuantity() const { return quantity_; }

    OrderPointer OrderModify::ToOrderPointer(OrderType type) const
    {
        return make_shared<Order>(type, GetOrderId(), GetPrice(), GetSide(), GetQuantity());
    }