#include <bits/stdc++.h>
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
    }
};

int main()
{

    return 0;
}