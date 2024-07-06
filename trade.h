#ifndef TRADE_H
#define TRADE_H

#include <chrono>

struct ExecutedTrade
{

    double price;
    int quantity;
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time;

    // Constructor
    ExecutedTrade(double prc, int qty)
        : price(prc),
          quantity(qty),
          start_time(std::chrono::high_resolution_clock::now()) // Initialize start_time
    {
    }
};

#endif // MARKET_H
