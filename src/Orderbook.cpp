#include "Orderbook.h"

#include "Orderbook.h"
#include <numeric>   // for std::accumulate
#include <algorithm> // for std::min

Orderbook::Orderbook() {}

bool Orderbook::CanMatch(Side side, Price price) const {
    if (side == Side::BUY) {
        if (asks_.empty()) return false;

        const auto& [bestAsk, _] = *asks_.begin();
        return price >= bestAsk;
    } 
    else { // SELL order
        if (bids_.empty()) return false;

        const auto& [bestBid, _] = *bids_.begin();
        return price <= bestBid;
    }
}

Trades Orderbook::MatchOrders() {
    Trades trades;
    trades.reserve(orders_.size());

    while (true) {
        if (bids_.empty() || asks_.empty())
            break;

        auto& [bidPrice, bidOrders] = *bids_.begin();
        auto& [askPrice, askOrders] = *asks_.begin();

        if (bidPrice < askPrice)
            break; // no crossing prices, no match

        while (!bidOrders.empty() && !askOrders.empty()) {
            auto& bid = bidOrders.front();
            auto& ask = askOrders.front();

            Quantity quantity = std::min(
                bid->GetRemainingQuantity(),
                ask->GetRemainingQuantity()
            );

            bid->Fill(quantity);
            ask->Fill(quantity);

            // remove filled bid
            if (bid->isFilled()) {
                orders_.erase(bid->GetOrderId());
                bidOrders.pop_front();
            }

            // remove filled ask
            if (ask->isFilled()) {
                orders_.erase(ask->GetOrderId());
                askOrders.pop_front();
            }

            // erase empty price levels
            if (bidOrders.empty())
                bids_.erase(bidPrice);

            if (askOrders.empty())
                asks_.erase(askPrice);

            // record trade
            trades.push_back(
                Trade(
                    TradeInfo{bid->GetOrderId(), bid->GetPrice(), quantity},
                    TradeInfo{ask->GetOrderId(), ask->GetPrice(), quantity}
                )
            );
        }
    }

    return trades;
}

Trades Orderbook::AddOrder(OrderPointer order) {
    // prevent duplicate order IDs
    if (orders_.contains(order->GetOrderId()))
        return {};

    // FOK rejected if not matchable
    if (order->GetOrderType() == OrderType::FillAndKill &&
        !CanMatch(order->GetSide(), order->GetPrice()))
        return {};

    OrderPointers::iterator it;

    if (order->GetSide() == Side::BUY) {
        auto& orderList = bids_[order->GetPrice()];
        orderList.push_back(order);
        it = std::prev(orderList.end());
    } 
    else {
        auto& orderList = asks_[order->GetPrice()];
        orderList.push_back(order);
        it = std::prev(orderList.end());
    }

    // store reference for cancel modifications
    orders_.insert({
        order->GetOrderId(),
        OrderEntry{order, it}
    });

    return MatchOrders();
}

void Orderbook::CancelOrder(OrderId orderId) {
    if (!orders_.contains(orderId))
        return;

    const auto& [order, it] = orders_.at(orderId);
    orders_.erase(orderId);

    Price price = order->GetPrice();

    if (order->GetSide() == Side::SELL) {
        auto& list = asks_.at(price);
        list.erase(it);
        if (list.empty()) asks_.erase(price);
    } 
    else {
        auto& list = bids_.at(price);
        list.erase(it);
        if (list.empty()) bids_.erase(price);
    }
}

Trades Orderbook::MatchOrders(OrderModify mod) {
    if (!orders_.contains(mod.GetOrderId()))
        return {};

    const auto& [existingOrder, _] = orders_.at(mod.GetOrderId());
    CancelOrder(mod.GetOrderId());

    return AddOrder(
        mod.ToOrderPointer(existingOrder->GetOrderType())
    );
}

size_t Orderbook::Size() const {
    return orders_.size();
}

OrderbookLevelInfos Orderbook::GetOrderInfos() const {
    LevelInfos bidInfos, askInfos;

    for (const auto& [price, orders] : bids_) {
        Quantity total = std::accumulate(
            orders.begin(), orders.end(),
            (Quantity)0,
            [](Quantity sum, const OrderPointer& o) {
                return sum + o->GetRemainingQuantity();
            }
        );
        bidInfos.push_back({price, total});
    }

    for (const auto& [price, orders] : asks_) {
        Quantity total = std::accumulate(
            orders.begin(), orders.end(),
            (Quantity)0,
            [](Quantity sum, const OrderPointer& o) {
                return sum + o->GetRemainingQuantity();
            }
        );
        askInfos.push_back({price, total});
    }

    return OrderbookLevelInfos(bidInfos, askInfos);
}

