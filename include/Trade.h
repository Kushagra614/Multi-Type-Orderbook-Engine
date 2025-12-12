#pragma once

#include<bits/stdc++.h>
#include<TradeInfo.h>

using namespace std;

class Trade
{
    public:
    Trade(const TradeInfo& BidTrade, const TradeInfo& AskTrade);
    
    const TradeInfo& GetBidTrade() const;
    const TradeInfo& GetAskTrade() const;

    private:
    TradeInfo bidTrade_;
    TradeInfo askTrade_;
    
};

using Trades = vector<Trade>;