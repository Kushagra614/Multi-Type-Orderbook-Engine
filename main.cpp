#include <iostream>
#include <iomanip>
#include <limits>
#include "Orderbook.h"
#include "Order.h"


void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void displayMainMenu() {
    std::cout << "\n=== Order Book System ===\n";
    std::cout << "1. Add Buy Order\n";
    std::cout << "2. Add Sell Order\n";
    std::cout << "3. Cancel Order\n";
    std::cout << "4. View Order Book\n";
    std::cout << "5. Exit\n";
    std::cout << "Enter your choice (1-5): ";
}

OrderType getOrderType() {
    int typeChoice;
    std::cout << "\n=== Select Order Type ===\n";
    std::cout << "1. Good Till Cancel (GTC) - Stays in book until filled or cancelled\n";
    std::cout << "2. Fill and Kill (FAK) - Fills as much as possible, cancels the rest\n";
    std::cout << "3. Fill or Kill (FOK) - Fills completely or not at all\n";
    std::cout << "4. Good For Day (GFD) - Active until end of trading day\n";
    std::cout << "5. Market Order - Executes at best available price\n\n";
    std::cout << "Enter order type (1-5): ";
    
    while (true) {
        std::cin >> typeChoice;
        clearInput();
        
        switch (typeChoice) {
            case 1: return OrderType::GoodTillCancel;
            case 2: return OrderType::FillAndKill;
            case 3: return OrderType::FillOrKill;
            case 4: return OrderType::GoodForDay;
            case 5: return OrderType::Market;
            default:
                std::cout << "Invalid choice. Please enter a number between 1 and 5: ";
        }
    }
}

int main() {
    Orderbook orderbook;
    int choice = 0;
    int orderId = 1; // Simple order ID counter
    
    std::cout << "Welcome to the Order Book System!\n";
    
    while (true) {
        displayMainMenu();
        std::cin >> choice;
        
        // Clear any remaining input
        clearInput();
        
        if (choice == 5) {
            std::cout << "Exiting...\n";
            break;
        }
        
        switch (choice) {
            case 1: // Add Buy Order
            case 2: { // Add Sell Order
                int quantity;
                double price;
                
                // Get order type
                OrderType type = getOrderType();
                
                // Get quantity
                std::cout << "Enter quantity: ";
                while (!(std::cin >> quantity) || quantity <= 0) {
                    std::cout << "Invalid quantity. Please enter a positive number: ";
                    clearInput();
                }
                
                // Get price
                std::cout << "Enter price: ";
                while (!(std::cin >> price) || price <= 0) {
                    std::cout << "Invalid price. Please enter a positive number: ";
                    clearInput();
                }
                
                // Create the order
                Side side = (choice == 1) ? Side::Buy : Side::Sell;
                auto order = std::make_shared<Order>(
                    type,  // Using the selected order type
                    orderId++, 
                    side, 
                    price, 
                    quantity
                );
                
                // Show order type specific messages
                // switch (type) {
                //     case OrderType::FillAndKill:
                //         std::cout << "(Fill and Kill order - will be cancelled if not immediately filled)\n";
                //         break;
                //     case OrderType::FillOrKill:
                //         std::cout << "(Fill or Kill order - will be cancelled if not completely filled)\n";
                //         break;
                //     case OrderType::Market:
                //         std::cout << "(Market order - will execute at best available price)\n";
                //         break;
                //     case OrderType::GoodForDay:
                //         std::cout << "(Good For Day order - will expire at end of trading day)\n";
                //         break;
                //     default:  // GoodTillCancel
                //         std::cout << "(Good Till Cancel order - remains active until filled or cancelled)\n";
                // }
                
                auto trades = orderbook.AddOrder(order);
                
                std::cout << (side == Side::Buy ? "Buy" : "Sell") 
                          << " order added with ID: " << (orderId - 1) << "\n";
                
                if (!trades.empty()) {
                    std::cout << "\nTrade(s) executed!\n";
                    for (const auto& trade : trades) {
                        std::cout << "  Matched Order " << trade.GetBidTrade().orderId_ 
                                << " (Buy) with Order " << trade.GetAskTrade().orderId_ << " (Sell)\n";
                        std::cout << "  Price: " << trade.GetBidTrade().price_ 
                                << ", Quantity: " << trade.GetBidTrade().quantity_ << "\n";
                    }
                }
                break;
            }
            
            case 3: { // Cancel Order
                int cancelId;
                std::cout << "Enter Order ID to cancel: ";
                while (!(std::cin >> cancelId) || cancelId < 1) {
                    std::cout << "Invalid Order ID. Please enter a positive number: ";
                    clearInput();
                }
                
                orderbook.CancelOrder(cancelId);
                std::cout << "Order " << cancelId << " cancelled (if it existed).\n";
                break;
            }
            
            case 4: { // View Order Book
                std::cout << "\n=== ORDER BOOK ===\n";
                
                // Get all order infos
                auto orderInfos = orderbook.GetOrderInfos();
                
                // Get bids and asks
                const auto& bids = orderInfos.GetBids();
                const auto& asks = orderInfos.GetAsks();
                
                // Display bids (buy orders) - highest price first
                if (!bids.empty()) {
                    std::cout << "\nBIDS (Buy Orders)\n";
                    std::cout << "----------------------------\n";
                    std::cout << "Price        Quantity\n";
                    std::cout << "----------------------------\n";
                    
                    for (auto it = bids.rbegin(); it != bids.rend(); ++it) {
                        const auto& level = *it;
                        std::cout << std::fixed << std::setprecision(2) << std::setw(10) << level.price_ 
                                 << "    " << std::setw(10) << level.quantity_ << "\n";
                    }
                } else {
                    std::cout << "\nNo BIDS in the order book.\n";
                }
                
                // Display asks (sell orders) - lowest price first
                if (!asks.empty()) {
                    std::cout << "\nASKS (Sell Orders)\n";
                    std::cout << "----------------------------\n";
                    std::cout << "Price        Quantity\n";
                    std::cout << "----------------------------\n";
                    
                    for (const auto& level : asks) {
                        std::cout << std::fixed << std::setprecision(2) << std::setw(10) << level.price_ 
                                 << "    " << std::setw(10) << level.quantity_ << "\n";
                    }
                } else {
                    std::cout << "\nNo ASKS in the order book.\n";
                }
                
                std::cout << "\nTotal active orders: " << orderbook.Size() << "\n\n";
                break;
            }
                
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
        
        // Clear any remaining input before next iteration
        clearInput();
    }
    
    return 0;
}