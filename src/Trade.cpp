#include "Trade.h"

Trade::Trade(const TradeInfo& BidTrade, const TradeInfo& AskTrade)
    : bidTrade_ { BidTrade }
    , askTrade_ { AskTrade }
    { }

    const TradeInfo& Trade::GetBidTrade() const {return bidTrade_; }
    const TradeInfo& Trade::GetAskTrade() const {return askTrade_; }