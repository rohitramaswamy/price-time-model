#ifndef TRADINGSTRATEGY_H
#define TRADINGSTRATEGY_H

#include <vector>
#include <utility>

class TradingStrategy {
public:
    virtual std::vector<std::pair<int, double>> generateSignals(const std::vector<double>& data) = 0;
    virtual ~TradingStrategy() = default;
};

#endif // TRADINGSTRATEGY_H
