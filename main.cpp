#include <iostream>
#include "include/Orderbook.h"
#include "include/Order.h"

int main() {
    // Creating orderbook
    Orderbook orderbook;

    const OrderId orderId = 1;

    // Add one BUY order
    orderbook.AddOrder(
        std::make_shared<Order>(OrderType::GoodTillCancel, orderId, Side::BUY, 100, 10)
    );

    std::cout << "Size of the Orderbook: " << orderbook.Size() <<" \n"; // expect 1

    // Cancel the order
    orderbook.CancelOrder(orderId);

    std::cout << "Size of the Orderbook: " << orderbook.Size() << " \n"; 

    return 0;
}
