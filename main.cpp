#include <bits/stdc++.h>
#include<algorithm>
using namespace std;

enum class OrderType
{
    GoodTillCancel,
    FillAndKill
};

enum class Side
{
    BUY,
    SELL
};

using Price = int32_t;
using Quantity = uint32_t;
using OrderId = uint64_t;

struct LevelInfo // this will get the info about the state or the OB from some public api
{
    Price price_;
    Quantity quantity_;
};

using LevelInfos = vector<LevelInfo>;

class OrderbookLevelInfos
{
public:
    OrderbookLevelInfos(const LevelInfos &bids, const LevelInfos &asks)
    {
        bids_ = bids;
        asks_ = asks;
    }

    // Adding some public APIs
    const LevelInfos &getBids() const { return bids_; }
    const LevelInfos &getAsks() const { return asks_; }

private:
    LevelInfos bids_;
    LevelInfos asks_;
};

class Order
{
public:
    Order(OrderType orderType, OrderId orderId, Side side, Price price, Quantity quantity)
        : orderType_{orderType}, orderId_{orderId}, side_{side}, price_{price}, initialQuantity_{quantity}, remainingQuantity_{quantity}

    { }

    OrderType GetOrderType() const { return orderType_; }
    OrderId GetOrderId() const { return orderId_; }
    Side GetSide() const { return side_; }
    Price GetPrice() const { return price_; }

    Quantity GetInitialQuantity() const { return initialQuantity_; }
    Quantity GetRemainingQuantity() const { return remainingQuantity_; }
    Quantity GetFilledQuantity() const { return GetInitialQuantity() - GetRemainingQuantity(); }


    bool isFilled() const
    {
        return GetRemainingQuantity() == 0;
    }

    void Fill(Quantity quantity)
    {
        if (quantity > GetRemainingQuantity())
        {
            throw logic_error(
                format("Order ({}) cannot be filled for more than its remaining quantity.",
                       GetOrderId()));
        }

        remainingQuantity_ -= quantity;
    }

private:
    OrderType orderType_;
    OrderId orderId_;
    Side side_;
    Price price_;
    Quantity initialQuantity_;
    Quantity remainingQuantity_;
};

using OrderPointer = shared_ptr<Order>;
using OrderPointers = list<OrderPointer>;

class OrderModify
{
public:
    OrderModify(OrderId orderId, Side side, Price price, Quantity quantity)
        : orderId_{orderId}, side_{side}, price_{price}, quantity_{quantity}

    { }

    // writing the API calls
    OrderId GetOrderId() const { return orderId_; }
    Side GetSide() const { return side_; }
    Price GetPrice() const { return price_; }
    Quantity GetQuantity() const { return quantity_; }

    OrderPointer ToOrderPointer(OrderType type) const
    {
        return make_shared<Order>(type, GetOrderId(), GetPrice(), GetSide(), GetQuantity());
    }

private:
    OrderId orderId_;
    Side side_;
    Price price_;
    Quantity quantity_;
};

//Now we will be working on the matching of the trades in the orderbook
struct TradeInfo
{
    OrderId orderid_;
    Price price_;
    Quantity quantity_;
};

class Trade
{
    public:
    Trade(const TradeInfo& BidTrade, const TradeInfo& AskTrade)
    : bidTrade_ { BidTrade }
    , askTrade_ { AskTrade }
    { }

    const TradeInfo& GetBidTrade() const {return bidTrade_; }
    const TradeInfo& GetAskTrade() const {return askTrade_; }

    private:
    TradeInfo bidTrade_;
    TradeInfo askTrade_;
    
};

using Trades = vector<Trade>;

class Orderbook
{
    private:
    struct OrderEntry
    {
        OrderPointer order_ { nullptr };
        OrderPointers::iterator location_;

    };

    map<Price, OrderPointers, greater<Price>> bids_;
    map<Price, OrderPointers, less<Price>> asks_;
    unordered_map<OrderId, OrderEntry> orders_;

    //now logic for matching 
    bool CanMatch(Side side, Price price) const
    {
        if(side == Side::BUY)
        {
            if(asks_.empty()) return false;

            // it will only match if the buy price is greater or equal to the ask price
            // finding the best ask price( lowest)
            const auto& [bestAsk, _] = *asks_.begin();
            return price >= bestAsk;  
        }
        else{
            if(bids_.empty()) return false;
            const auto [bestBid, _] = *bids_.begin();
            return price <=  bestBid;
         }
        
    }

    Trades MatchOrders()
    {
        Trades trades;
        trades.reserve(orders_.size());

        while(true)
        {
            if(bids_.empty() || asks_.empty()) break;

            // get the bid and ask values
            auto& [bidPrice, bids]  = *bids_.begin();
            auto& [askPrice, asks] = *asks_.begin();

            if(bidPrice > askPrice) break;

            while(bids.size() && asks.size())
            {
                auto& bid = bids.front();
                auto& ask = asks.front();

                //quant remaininig 
                Quantity quantity = min(bid->GetRemainingQuantity(), ask->GetRemainingQuantity());

                bid->Fill(quantity);
                ask->Fill(quantity);

                if(bid->isFilled())
                {
                    bids.pop_front();
                    orders_.erase(bid->GetOrderId());
                }

                if(ask->isFilled())
                {
                    asks.pop_front();
                    orders_.erase(ask->GetOrderId());
                }

                if(bids_.empty())
                {
                    bids_.erase(bidPrice);

                }

                if(asks_.empty())
                {
                    asks_.erase(askPrice);
                }

                trades.push_back(Trade{
                    TradeInfo{bid->GetOrderId(), bid->GetPrice(), quantity },
                    TradeInfo{ask->GetOrderId(), ask->GetPrice(), quantity }
                });
            }
        }

        //Now we will be removing the remaining quant of the FOK orders once it has been partially filled
        if(bids_.empty())
        {
            //get the order-> bid
            auto& [_, bids] = *bids_.begin();
            auto& order = bids.front();
            if(order->GetOrderType() == OrderType::FillAndKill)
            {
                CancelOrder(order->GetOrderId());
            }
                
        }

        if(asks_.empty())
        {
            //get the order-> bid
            auto& [_, asks] = *asks_.begin();
            auto& order = asks.front();
            if(order->GetOrderType() == OrderType::FillAndKill)
            {
                CancelOrder(order->GetOrderId());
            }
                
        }

        return trades;
    }

    public:
        Trades AddOrder(OrderPointer order)
        {
            //1st we will go thru the exit cases
            if(orders_.contains(order->GetOrderId()))  return { };

            //if a FOK order can't be filled then no point adding it
            if(order->GetOrderType() == OrderType::FillAndKill && !CanMatch(order->GetSide(), order->GetPrice()))
            {
                return { };
            }

            OrderPointers::iterator iterator;

            if(order->GetSide() == Side::BUY)
            {
                auto& orders = bids_[order->GetPrice()];
                orders.push_back(order);
                iterator = next(orders.begin(), orders.size()-1);
            }
            else
            {
                auto& orders = asks_[order->GetPrice()];
                orders.push_back(order);
                iterator = next(orders.begin(), orders.size()-1);
            }

            // Inserting this into another data structure
            orders_.insert({order->GetOrderId(), OrderEntry(order, iterator)});
            return MatchOrders();
        }

        void CancelOrder(OrderId orderId)
        {
            if(!orders_.contains(orderId))
            {
                return ;
            }

            //OrderId does exist
            const auto& [order, iterator] = orders_.at(orderId);
            orders_.erase(orderId);

            if(order->GetSide() == Side::SELL)
            {
                auto price = order->GetPrice();
                auto& orders = asks_.at(price);
                orders.erase(iterator);
                if(orders.empty())
                {
                    asks_.erase(price);
                }
            }
            else
            {
                auto price = order->GetPrice();
                auto& orders = bids_.at(price);
                orders.erase(iterator);
                if(orders.empty())
                {
                    bids_.erase(price);
                }
            }
        }

            Trades MatchOrders(OrderModify order)
            {
                if(!orders_.contains(order.GetOrderId()))
                {
                    return { };
                }

                const auto& [existingOrder, _] = orders_.at(order.GetOrderId());
                CancelOrder(order.GetOrderId());
                return AddOrder(order.ToOrderPointer(existingOrder->GetOrderType()));
            }

            size_t Size() const {return orders_.size(); }

            OrderbookLevelInfos GetOrderInfos() const
            {
                LevelInfos bidInfos, askInfos;
                bidInfos.reserve(orders_.size());
                askInfos.reserve(orders_.size());   

                //creating a lambda fn
                auto CreateLevelInfos = [](Price price, const OrderPointers& orders)
                {
                    return LevelInfo{price, accumulate(orders.begin(), orders.end(), (Quantity)0, 
                        [](Quantity runningSum, const OrderPointer& order)
                        {return runningSum + order->GetRemainingQuantity();}) };
                };

                for(const auto& [price, orders] : bids_ )
                {
                    bidInfos.push_back(CreateLevelInfos(price, orders));
                }
                
                for(const auto& [price, orders] : asks_)
                {
                    bidInfos.push_back((CreateLevelInfos(price, orders)));
                }

                return OrderbookLevelInfos(bidInfos, askInfos);

            }
};

int main()
{
    //creating orderbook
    Orderbook orderbook;
    const OrderId orderId = 1;
    orderbook.AddOrder(make_shared<Order>(OrderType::GoodTillCancel, orderId, Side::BUY, 100 ,10));
    cout<<"Size of the Orderbook: "<< orderbook.Size()<<endl; // 1
    orderbook.CancelOrder(orderId);
    cout<<"Size of the Orderbook: "<<orderbook.Size()<<endl; // 0

    return 0;
}