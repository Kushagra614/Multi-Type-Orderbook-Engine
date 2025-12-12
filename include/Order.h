#pragma once

#include <memory>  
#include <list>

#include "Constants.h"
#include "Side.h"
#include "OrderType.h"

class Order
{
public:
    Order(OrderType orderType, OrderId orderId, Side side, Price price, Quantity quantity);
    
    OrderType GetOrderType() const;
    OrderId GetOrderId() const;
    Side GetSide() const;
    Price GetPrice() const;

    Quantity GetInitialQuantity() const ;
    Quantity GetRemainingQuantity() const;
    Quantity GetFilledQuantity() const;


    bool isFilled() const;
    void Fill(Quantity quantity);
    

private:
    OrderType orderType_;
    OrderId orderId_;
    Side side_;
    Price price_;
    Quantity initialQuantity_;
    Quantity remainingQuantity_;
};

using OrderPointer = std::shared_ptr<Order>;
using OrderPointers = std::list<OrderPointer>;

