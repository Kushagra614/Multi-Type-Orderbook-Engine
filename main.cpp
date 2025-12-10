#include<bits/stdc++.h>
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
    OrderbookLevelInfos(const LevelInfos& bids, const LevelInfos& asks)
    {
        bids_ = bids;
        asks_ = asks;
    }

    //Adding some public APIs
    const LevelInfos& getBids() const {return bids_;}
    const LevelInfos& getAsks() const {return asks_;}

    private:
    LevelInfos bids_;
    LevelInfos asks_;
};

class Order
{
    public:
    Order(Ordertype orderType, OrderId orderId, Side side, Price price, Quatity quantity)
    : orderType_{ orderType}
    , orderId_{orderId}
    , side_{side}
    , price_{price}
    , initialQuantity_{quantity}
    , remainingQuantity{quantity}

    { }

    OrderType GetOrderType() const {return orderType_; }
    OrderId GetOrderId() const {return orderId_; }
    Side GetSide() const {return side_; }
    Price GetPrice() const {return price_; }
    Quantity GetInitialQuantity() const {return initialQuanity_; }
    Quantity GetRemainingQuantity() const {return remainingQuantity_; }
    
};

int main()
{
    
    return 0;
}