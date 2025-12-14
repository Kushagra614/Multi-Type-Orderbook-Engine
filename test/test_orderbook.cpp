#include <gtest/gtest.h>
#include <string>
#include <memory>

#include "Orderbook.h"
#include "Order.h"
#include "OrderType.h"
#include "Side.h"

class OrderbookTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Initialize common test data
        buyOrder = std::make_shared<Order>(OrderType::GoodTillCancel, 1, Side::Buy, 100, 10);
        sellOrder = std::make_shared<Order>(OrderType::GoodTillCancel, 2, Side::Sell, 100, 10);
    }

    Orderbook orderbook;
    std::shared_ptr<Order> buyOrder;
    std::shared_ptr<Order> sellOrder;
};

// Test that a new orderbook is empty
TEST_F(OrderbookTest, NewOrderbookIsEmpty) {
    EXPECT_EQ(orderbook.Size(), 0);
}

// Test adding a buy order
TEST_F(OrderbookTest, AddBuyOrder) {
    auto trades = orderbook.AddOrder(buyOrder);
    EXPECT_EQ(orderbook.Size(), 1);
    EXPECT_EQ(trades.size(), 0); // No trades should be generated
}

// Test adding a sell order
TEST_F(OrderbookTest, AddSellOrder) {
    auto trades = orderbook.AddOrder(sellOrder);
    EXPECT_EQ(orderbook.Size(), 1);
    EXPECT_EQ(trades.size(), 0); // No trades should be generated
}



// Test matching buy and sell orders
TEST_F(OrderbookTest, MatchOrders) {
    // Add buy order
    orderbook.AddOrder(buyOrder);
    
    // Add matching sell order
    auto trades = orderbook.AddOrder(sellOrder);
    
    // Should have one trade
    EXPECT_EQ(trades.size(), 1);
    if (!trades.empty()) {
        // Access the first trade and check its bid and ask trade info
        const auto& trade = trades[0];
        EXPECT_EQ(trade.GetBidTrade().price_, 100);
        EXPECT_EQ(trade.GetBidTrade().quantity_, 10);
        // The ask trade should also have the same price and quantity
        EXPECT_EQ(trade.GetAskTrade().price_, 100);
        EXPECT_EQ(trade.GetAskTrade().quantity_, 10);
    }
    // Both orders should be matched and removed
    EXPECT_EQ(orderbook.Size(), 0);
}



// Test canceling an order
TEST_F(OrderbookTest, CancelOrder) {
    orderbook.AddOrder(buyOrder);
    EXPECT_EQ(orderbook.Size(), 1);
    
    orderbook.CancelOrder(buyOrder->GetOrderId());
    EXPECT_EQ(orderbook.Size(), 0);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}