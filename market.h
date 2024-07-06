#ifndef MARKET_H
#define MARKET_H

#include <iostream>
#include <vector>
#include <queue>
#include <chrono>
#include <random>
#include <mutex>
#include <thread>
#include <atomic>
#include <fstream>

#include "order.h"        // Make sure to include the Order header file
#include "marketMakers.h" // Include MarketMakers header file if exists
#include <unordered_map>

#include "bot.h"
#include "trade.h"

using PriceTimestampPair = std::pair<double, std::chrono::time_point<std::chrono::high_resolution_clock>>;

struct CompareBuyOrder
{
    bool operator()(const Order &a, const Order &b)
    {
        // Custom comparison logic (e.g., compare by price)
        if (a.getPrice() != b.getPrice())
        {
            return a.getPrice() < b.getPrice(); // Min-heap based on price
        }
        else
        {
            return a.getTime() < b.getTime();
        }
    }
};

struct CompareSellOrder
{
    bool operator()(const Order &a, const Order &b)
    {
        // Custom comparison logic (e.g., compare by price)
        if (a.getPrice() != b.getPrice())
        {
            return a.getPrice() > b.getPrice(); // Min-heap based on price
        }
        else
        {
            return a.getTime() < b.getTime();
        }
    }
};

class Market
{
private:
    std::vector<ExecutedTrade> order_book;
    std::vector<MarketMakers> market_makers;

    double highest_bid;
    double lowest_ask;

    std::unordered_map<int, Bots> bots;

    // Queues to store timestamps of buy and sell orders

public:
    // Constructor
    std::priority_queue<Order, std::vector<Order>, CompareBuyOrder> buy_limit_orders;
    std::priority_queue<Order, std::vector<Order>, CompareSellOrder> sell_limit_orders;

    std::unordered_map<int, int> order_quantity;

    Market(std::vector<MarketMakers> MM);

    std::queue<std::chrono::time_point<std::chrono::high_resolution_clock>> buy_timestamps;
    std::queue<std::chrono::time_point<std::chrono::high_resolution_clock>> sell_timestamps;

    std::vector<double> market_data;

    std::queue<PriceTimestampPair> price_timestamps_queue;

    double current_buy_rate;
    double current_sell_rate;

    double price_rate;

    // utility functions

    // boolean based on probability
    static bool getRandomBoolean(double number);
    static double getRandomDouble(double min, double max);
    static int getRandomInteger(int min, int max);

    // market functions
    void check_limit_orders();
    void execute_trade(double price, int trade_quantity);
    void simulate_market(int seconds, int num_bots, int trades_per_bot);

    double calculate_buy_rate(std::chrono::seconds interval);
    double calculate_sell_rate(std::chrono::seconds interval);

    double change_in_buy_rate();
    double change_in_sell_rate();
    double change_in_price(std::chrono::seconds interval);

    void check_orders_and_spread(int seconds, std::atomic<bool> &running);
    double calculate_highest_bid();
    double calculate_lowest_ask();
    void regenerate_spread();

    void add_to_buy_limit_queue(Order &o);
    void add_to_sell_limit_queue(Order &o);

    double getHighestBid();
    double getLowestAsk();
};

#endif // MARKET_H
