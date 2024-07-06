#ifndef MARKETMAKERS_H
#define MARKETMAKERS_H
#include <string>
#include "market.h"

class MarketMakers 
{
private:
    double bid;
    double ask;
    double profit;

public:
    // Constructor
    MarketMakers(double bid, double ask) : bid(bid), ask(ask), profit(0.0) {}

    // Method to change the spreads
    void adjust_spread_for_comp(Market &m, char strategy);

    // Accessor methods
    double getBid() const { return bid; }
    double getAsk() const { return ask; }
    double getProfit() const { return profit; }

    // Mutator methods
    void setBid(double new_bid) { bid = new_bid; }
    void setAsk(double new_ask) { ask = new_ask; }
    void setProfit(double new_profit) { profit = new_profit; }
};

#endif // MARKETMAKERS_H
