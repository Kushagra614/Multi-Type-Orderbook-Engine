#include "OrderbookLevelInfos.h"

OrderbookLevelInfos::OrderbookLevelInfos(const LevelInfos &bids, const LevelInfos &asks)
    {
        bids_ = bids;
        asks_ = asks;
    }

    // Adding some public APIs
    const LevelInfos &OrderbookLevelInfos::getBids() const { return bids_; }
    const LevelInfos &OrderbookLevelInfos::getAsks() const { return asks_; }