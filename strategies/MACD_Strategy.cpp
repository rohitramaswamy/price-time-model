#include "MACD_Strategy.h"

std::vector<double> MACDStrategy::calculateEMA(const std::vector<double> &data, int period)
{
    std::vector<double> ema(data.size(), 0.0);
    double multiplier = 2.0 / (period + 1);
    ema[0] = data[0];
    for (size_t i = 1; i < data.size(); ++i)
    {
        ema[i] = ((data[i] - ema[i - 1]) * multiplier) + ema[i - 1];
    }
    return ema;
}

std::vector<std::pair<int, double>> MACDStrategy::generateSignals(const std::vector<double> &data)
{
    std::vector<std::pair<int, double>> signals(data.size(), {0, 0.0});
    if (data.size() < 26)
        return signals;

    auto ema12 = calculateEMA(data, 12);
    auto ema26 = calculateEMA(data, 26);

    std::vector<double> macd(data.size(), 0.0);
    for (size_t i = 0; i < data.size(); ++i)
    {
        macd[i] = ema12[i] - ema26[i];
    }

    auto signalLine = calculateEMA(macd, 9);

    for (size_t i = 1; i < data.size(); ++i)
    {
        if (macd[i] > signalLine[i] && macd[i - 1] <= signalLine[i - 1])
        {
            signals[i] = {1, data[i]}; // Buy signal
        }
        else if (macd[i] < signalLine[i] && macd[i - 1] >= signalLine[i - 1])
        {
            signals[i] = {-1, data[i]}; // Sell signal
        }
    }

    return signals;
}
