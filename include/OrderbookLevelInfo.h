#pragma once

#include<bits/stdc++.h>
#include<LevelInfo.h>

using namespace std;

class OrderbookLevelInfos
{
public:
    OrderbookLevelInfos(const LevelInfos &bids, const LevelInfos &asks);
    
    // Adding some public APIs
    const LevelInfos &getBids() const;
    const LevelInfos &getAsks() const;

private:
    LevelInfos bids_;
    LevelInfos asks_;
};


