#ifndef BOTS_H
#define BOTS_H

#include <mutex>
#include <atomic>
#include "TradingStrategy.h"

class Market;

class Bots
{
private:
    double cash;
    int assets;
    int bot_id;

    int trading_frequency;

    std::vector<int> trade_ids;

public:
    Bots(double money, int stuff, int id);

    void bot_trade(Market &m, std::mutex &vec_mutex, int num_trades, double market_seconds, std::atomic<bool> &running, TradingStrategy &s);

    double getCash() const;
    int getAssets() const;
    void setAssets(int a);
    void setCash(double c);

    void BuyMarketValue();
    void SellMarketValue();

    void PlaceLimitOrders(std::vector<std::pair<int, double>> signal)
};

#endif // BOTS_H
