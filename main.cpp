#include <iostream>
#include "Orderbook.h"
#include "Order.h"

int main() {
    // Create an order book
    Orderbook orderbook;
    
    std::cout << "=== Simple Order Book Demo ===\n";
    
    // Add a buy order
    std::cout << "\nAdding a buy order (ID: 1, Price: 100, Quantity: 10)\n";
    orderbook.AddOrder(
        std::make_shared<Order>(OrderType::GoodTillCancel, 1, Side::Buy, 100, 10)
    );
    
    std::cout << "Order book size: " << orderbook.Size() << " order(s)\n";
    
    // Add a sell order
    std::cout << "\nAdding a sell order (ID: 2, Price: 100, Quantity: 5)\n";
    auto trades = orderbook.AddOrder(
        std::make_shared<Order>(OrderType::GoodTillCancel, 2, Side::Sell, 100, 5)
    );
    
    // Show trades that were executed
    if (!trades.empty()) {
        std::cout << "\nTrade executed!\n";
        for (const auto& trade : trades) {
            std::cout << "  Matched Order " << trade.GetBidTrade().orderId_ 
                      << " (Buy) with Order " << trade.GetAskTrade().orderId_ << " (Sell)\n";
            std::cout << "  Price: " << trade.GetBidTrade().price_ 
                      << ", Quantity: " << trade.GetBidTrade().quantity_ << "\n";
        }
    }
    
    std::cout << "\nOrder book size after matching: " << orderbook.Size() << " order(s)\n";
    
    // Cancel the remaining order
    std::cout << "\nCancelling remaining orders...\n";
    orderbook.CancelOrder(1);  // Cancel the remaining buy order
    
    std::cout << "Final order book size: " << orderbook.Size() << " order(s)\n";
    
    return 0;
}