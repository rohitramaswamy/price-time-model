#include "RSI_Strategy.h"
#include <numeric>
#include <algorithm>

double RSIStrategy::calculateRSI(const std::vector<double>& data, int period) {
    if (data.size() < period) return 0.0;
    double gain = 0.0, loss = 0.0;
    for (size_t i = 1; i < period; ++i) {
        double change = data[i] - data[i - 1];
        if (change > 0) {
            gain += change;
        } else {
            loss -= change;
        }
    }
    gain /= period;
    loss /= period;
    double rs = gain / loss;
    return 100 - (100 / (1 + rs));
}

std::vector<std::pair<int, double>> RSIStrategy::generateSignals(const std::vector<double>& data) {
    std::vector<std::pair<int, double>> signals(data.size(), {0, 0.0});
    int period = 14;
    for (size_t i = period; i < data.size(); ++i) {
        std::vector<double> slice(data.begin() + i - period, data.begin() + i);
        double rsi = calculateRSI(slice, period);
        if (rsi > 70) {
            signals[i] = {-1, data[i]};  // Sell signal
        } else if (rsi < 30) {
            signals[i] = {1, data[i]};   // Buy signal
        }
    }
    return signals;
}
