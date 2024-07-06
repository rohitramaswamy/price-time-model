#include "MovingAveragesStrategy.h"
#include <numeric>

std::vector<double> MovingAverageStrategy::calculateSMA(const std::vector<double> &data, int period)
{
    std::vector<double> sma(data.size(), 0.0);
    for (size_t i = period - 1; i < data.size(); ++i)
    {
        sma[i] = std::accumulate(data.begin() + i - period + 1, data.begin() + i + 1, 0.0) / period;
    }
    return sma;
}

std::vector<std::pair<int, double>> MovingAverageStrategy::generateSignals(const std::vector<double> &data)
{
    std::vector<std::pair<int, double>> signals(data.size(), {0, 0.0});
    int shortPeriod = 10;
    int longPeriod = 50;

    if (data.size() < longPeriod)
        return signals;

    auto shortSMA = calculateSMA(data, shortPeriod);
    auto longSMA = calculateSMA(data, longPeriod);

    for (size_t i = longPeriod - 1; i < data.size(); ++i)
    {
        if (shortSMA[i] > longSMA[i] && shortSMA[i - 1] <= longSMA[i - 1])
        {
            signals[i] = {1, data[i]}; // Buy signal
        }
        else if (shortSMA[i] < longSMA[i] && shortSMA[i - 1] >= longSMA[i - 1])
        {
            signals[i] = {-1, data[i]}; // Sell signal
        }
    }

    return signals;
}
