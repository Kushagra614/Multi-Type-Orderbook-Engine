#pragma once

#include<bits/stdc++.h>
#include<Constants.h>
using namespace std;

struct LevelInfo // this will get the info about the state or the OB from some public api
{
    Price price_;
    Quantity quantity_;
};

using LevelInfos = vector<LevelInfo>;